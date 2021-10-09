#include "stringssearch.h"

/*!
 * @fn StringSearch::remove_symbols
 * @brief Removes punctuation.
 * @param[in] str string that should not contain punctuation
 * @returns string without punctuation
 */
QString StringSearch::remove_symbols(QString str) {
  for (auto symbol : ".!?;:,-'\"")
    str.remove(symbol);
  return str;
}

/*!
 * @fn StringSearch::purify
 * @brief Purifies @a str.
 * @param[in] str string to be purified
 * @returns purified string
 */
QString StringSearch::purify(const QString &str) {
  return remove_symbols(str.trimmed().toLower());
}

/*!
 * @fn StringSearch::get_POC
 * @brief Calculates the percentage of the second word in the first.
 * @param[in] e1 source word
 * @param[in] e2 search word
 * @returns % of @a e2 in @a e1
 */
float StringSearch::get_POC(const QString &e1, const QString &e2) {
  if (e1.length() + e2.length() == 0)
    return 0.0;
  QList<int> indices;
  int i = 0;
  for (int j = 0; j < e2.length(); j++)
    if ((i = e1.indexOf(e2[j], j)) != -1 and
        i <= j + (e1.length() - e2.length()))
      indices.append(i);
  if (not indices.length())
    return 0.0;
  QList<QList<int>> subs;
  for (auto i : indices) {
    for (int si = 0; si < subs.length(); si++)
      if (subs[si].length())
        if (subs[si].last() < i)
          subs[si].append(i);
    subs.append(QList<int>());
    subs[subs.length() - 1].append(i);
  }
  int max_len = 0, len = 0;
  for (int si = 0; si < subs.length(); si++)
    if ((len = subs[si].length()) > max_len)
      max_len = len;
  return float(max_len * 2) / (e1.length() + e2.length());
}

/*!
 * @fn StringSearch::contains
 * @brief Determines the presence of the semantic load @a inner in this
 * expression.
 * @param[in] that input
 * @param[in] inner suggestable semantic load
 * @param[in] EL entry limit condition
 * @returns indices of semantic load or zeros
 * @sa get_POC
 */
QMap<int, int> StringSearch::contains(QString that, QString inner, float EL) {
  QMap<int, int> m;
  QList<WordMetadata> that_metadata, inner_metadata;
  auto p1 = purify(that), p2 = purify(inner);
  if (p1.length() < p2.length()) {
    auto x = p1;
    p1 = p2;
    p2 = x;
  }
  int last = 0;
  for (auto word : p1.split(" ")) {
    WordMetadata wmd;
    wmd.word = word;
    wmd.i1 = p1.indexOf(word, last);
    wmd.i2 = wmd.i1 + word.length();
    last = wmd.i2;
    that_metadata.append(wmd);
  }
  last = 0;
  for (auto word : p2.split(" ")) {
    WordMetadata wmd;
    wmd.word = word;
    wmd.i1 = p2.indexOf(word, last);
    wmd.i2 = wmd.i1 + word.length();
    last = wmd.i2;
    inner_metadata.append(wmd);
  }
  if (inner_metadata.length() == 0) {
    m[0] = 0;
    return m;
  }
  // TODO Этот алгоритм не шибко оптимален по скорости, но вполне
  // работоспособен.
  // FIXME Алгоритм не гарантирует сопоставимость по количеству выбранных
  // слов. Он вполне сработает с одним POC на два слова из inner.
  QList<QPair<float, WordMetadata>> common;
  for (auto w2 : inner_metadata) {
    QPair<float, WordMetadata> max_POC = {0.0, WordMetadata()};
    // TODO Можно ещё сделать реализацию POC по синонимам.
    for (auto w1 : that_metadata) {
      float POC = get_POC(w1.word, w2.word);
      if (POC >= EL and POC > max_POC.first)
        max_POC = QPair<float, WordMetadata>(POC, w1);
    }
    common.append(max_POC);
  }
  if (float(common.length()) / inner_metadata.length() >= EL) {
    m[0] = -2;
    for (auto wmd_pair : common)
      m[wmd_pair.second.i1] = wmd_pair.second.i2;
    return m;
  }
  m[0] = 0;
  return m;
}

/*!
 * @fn StringSearch::intersects
 * @brief Returns the intersection of expressions, calculating which one
 * covers more text.
 * @param[in] first indices to calculate intersection
 * @param[in] second indices to calculate intersection
 * @returns info about intersection
 */
Intersects StringSearch::intersects(QMap<int, int> first,
                                    QMap<int, int> second) {
  int first_total = 0;
  bool is_intersects = false;
  for (auto i : first.keys()) {
    if (i == 0 and first[i] == -2)
      continue;
    for (auto j : second.keys()) {
      if (j == 0 and second[j] == -2)
        continue;
      first_total += first[i] - i - second[j] + j;
      if (i == j) {
        is_intersects = true;
      } else if (i < j) {
        if (first[i] < j)
          continue;
        else
          is_intersects = true;
      } else {
        if (i > second[j])
          continue;
        else
          is_intersects = true;
      }
    }
  }
  if (not is_intersects)
    return Intersects::No;
  else if (first_total == 0)
    return Intersects::Equal;
  else if (first_total > 0)
    return Intersects::FirstBetter;
  else
    return Intersects::SecondBetter;
}

/*!
 * @fn StringSearch::replace
 * @brief Replaces an expression in a string with another, given an accuracy
 * and inaccuracy search.
 * @param[in,out] that string in which you need to replace part of @a that
 * with @a to
 * @param[in] indices of replaceable @a that
 * @param[in] to <-- see above >
 * @returns @a that string with replaced text
 */
QString StringSearch::replace(QString that, QMap<int, int> indices,
                              QString to) {
  int len = that.length();
  for (auto i1 : indices.keys()) {
    if (i1 == 0 and indices[i1] == -2)
      continue;
    if (i1 > len or indices[i1] > len)
      continue;
    QString n = "";
    for (int i = i1; i < indices[i1]; i++)
      n += " ";
    auto left = that.left(i1);
    auto right = that.right(len - indices[i1]);
    that = left + n + right;
  }
  return that.trimmed();
}
