#include "stringssearch.h"

/*! @brief Determines the presence of the semantic load @a inner in this expression.
 *  @param[in] that input
 *  @param[in] inner suggestable semantic load
 *  @param[in] EL entry limit condition
 *  @param[in] HA handle asterics (usable for AIML)
 *  @returns indices of semantic load or zeros  */
QMap<int, int> StringSearch::contains(QString that, QString inner, float EL, bool HA) {
  QMap<int, int> m;
  QList<WordMetadata> that_metadata, inner_metadata;
  auto p1 = lemmatize(that), p2 = lemmatize(inner);
  if (p1.length() < p2.length()) {
    auto x = p1;
    p1 = p2;
    p2 = x;
  }
  int last = 0;
  for (auto word : p1.split(" ")) {
    WordMetadata wmd;
    wmd.word = word;
    wmd.i1 = p1.indexOf(wmd.word, last);
    wmd.i2 = wmd.i1 + wmd.word.length();
    last = wmd.i2;
    that_metadata.append(wmd);
  }
  last = 0;
  for (auto word : p2.split(" ")) {
    WordMetadata wmd;
    if (HA and (word == "*" or word == "_")) wmd.word = locate(p1, last);
    else wmd.word = word;
    wmd.i1 = p2.indexOf(wmd.word, last);
    wmd.i2 = wmd.i1 + wmd.word.length();
    last = wmd.i2;
    inner_metadata.append(wmd);
  }
  if (inner_metadata.length() == 0) {
    m[0] = 0;
    return m;
  }
  QList<QPair<float, WordMetadata>> common;
  for (auto w2 : inner_metadata) {
    QPair<float, WordMetadata> max_POC = {0.0, WordMetadata()};
    // TODO Можно ещё сделать реализацию POC по синонимам.
    for (auto w1 : that_metadata) {
      float POC = get_POC(w1.word, w2.word);
      if (POC >= EL and POC > max_POC.first) max_POC = QPair<float, WordMetadata>(POC, w1);
    }
    common.append(max_POC);
  }
  if (float(common.length()) / inner_metadata.length() >= EL) {
    m[0] = -2;
    for (auto wmd_pair : common) m[wmd_pair.second.i1] = wmd_pair.second.i2;
    return m;
  }
  m[0] = 0;
  return m;
}

/*! @brief Removes punctuation. */
QString StringSearch::remove_symbols(QString str) {
  for (auto symbol : ".!?;:,-'\"") str.remove(symbol);
  return str;
}

/*! @brief Purifies @a str. */
QString StringSearch::lemmatize(const QString &str) { return remove_symbols(str.simplified().toLower()); }

/*! @brief Calculates the percentage of the second word in the first. */
float StringSearch::get_POC(const QString &e1, const QString &e2) {
  if (e1.length() + e2.length() == 0) return 0.0;
  QList<int> indices;
  int i = 0;
  for (int j = 0; j < e2.length(); j++)
    if ((i = e1.indexOf(e2[j], j)) != -1 and i <= j + (e1.length() - e2.length())) indices.append(i);
  if (not indices.length()) return 0.0;
  QList<QList<int>> subs;
  for (auto i : indices) {
    for (int si = 0; si < subs.length(); si++)
      if (subs[si].length())
        if (subs[si].last() < i) subs[si].append(i);
    subs.append(QList<int>());
    subs[subs.length() - 1].append(i);
  }
  int max_len = 0, len = 0;
  for (int si = 0; si < subs.length(); si++) if ((len = subs[si].length()) > max_len) max_len = len;
  return float(max_len * 2) / (e1.length() + e2.length());
}

/*! @brief Returns the intersection of expressions, calculating which one covers more text. */
QPair<Intersects, int> StringSearch::intersects(QMap<int, int> first, QMap<int, int> second) {
  int first_total = 0;
  bool is_intersects = false;
  for (auto i : first.keys()) {
    if (i == 0 and first[i] == -2) continue;
    for (auto j : second.keys()) {
      if (j == 0 and second[j] == -2) continue;
      first_total += first[i] - i - second[j] + j;
      if (i == j) { is_intersects = true; }
      else if (i < j) {
        if (first[i] < j) continue;
        else is_intersects = true;
      } else {
        if (i > second[j]) continue;
        else is_intersects = true;
      }
    }
  }
  if (not is_intersects) return QPair<Intersects, int>(Intersects::No, 0);
  else if (first_total == 0) return QPair<Intersects, int>(Intersects::Equal, 0);
  else if (first_total > 0) return QPair<Intersects, int>(Intersects::FirstBetter, first_total);
  else return QPair<Intersects, int>(Intersects::SecondBetter, first_total);
}

/*! @brief Replaces an expression in a string with another, given an accuracy and inaccuracy search. */
QString StringSearch::replace(QString that, QMap<int, int> indices, QString to) {
  int len = that.length();
  for (auto i1 : indices.keys()) {
    if (i1 == 0 and indices[i1] == -2) continue;
    if (i1 > len or indices[i1] > len) continue;
    QString n = "";
    for (int i = i1; i < indices[i1]; i++) n += " ";
    auto left = that.left(i1);
    auto right = that.right(len - indices[i1]);
    that = left + n + right;
  }
  return that.trimmed();
}

/*! @brief Looking for the next word. */
QString StringSearch::locate(const QString &that, int last_index) {
  auto i = last_index + 1;
  if (i >= that.length()) return "";
  auto j = that.indexOf(' ', i);
  if (j == -1) return "";
  QString located;
  for (int k = i; k < j; k++) located += that[k];
  return located;
}
