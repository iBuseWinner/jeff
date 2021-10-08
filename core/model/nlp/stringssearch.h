#ifndef STRINGSSEARCH_H
#define STRINGSSEARCH_H

#include <QList>
#include <QPair>
#include <QString>
#include <iostream>

struct WordMetadata {
  int i1 = 0;
  int i2 = 0;
  QString word;
};

class StringSearch {
public:
  /*!
   * @fn StringSearch::remove_symbols
   * @brief Removes punctuation.
   * @param[in] str string that should not contain punctuation
   * @returns string without punctuation
   */
  static QString remove_symbols(QString str) {
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
  static QString purify(const QString &str) {
    return remove_symbols(str.trimmed().toLower());
  }

  /*!
   * @fn StringSearch::get_POC
   * @brief Calculates the percentage of the second word in the first.
   * @param[in] e1 source word
   * @param[in] e2 search word
   * @returns % of @a e2 in @a e1
   */
  static float get_POC(const QString &e1, const QString &e2) {
    std::cout << "--------" << std::endl;
    std::cout << "\tfn get_POC" << std::endl;
    std::cout << "\tGot e1 as \"" << e1.toStdString() << "\" and e2 as \""
              << e2.toStdString() << "\"" << std::endl;
    auto l1 = e1.length();
    auto l2 = e2.length();
    std::cout << "\tCalculated l1 = " << l1 << " and l2 = " << l2 << std::endl;
    if (l1 + l2 == 0)
      return 0.0;
    int i1 = 0, i2 = 0;
    int POC = 0;
    auto pause = false;
    QList<int> POCs;
    POCs.append(0);
    while (true) {
      auto f = false;
      if (e1[i1] == e2[i2]) {
        std::cout << "\te1[i1] == e2[i2]" << std::endl;
        if (pause) {
          pause = false;
          std::cout << "\tResuming with POC = " << POC << " -> 0" << std::endl;
          POCs.append(POC);
          POC = 0;
        }
        POC += 2;
        std::cout << "\tCalculated curr POC = " << POC << std::endl;
        if (i1 < l1 - 1)
          i1++;
        else
          f = true;
        if (i2 < l2 - 1)
          i2++;
        else
          f = true;
        if (f) {
          POCs.append(POC);
          break;
        }
      } else {
        std::cout << "\te1[i1] != e2[i2]" << std::endl;
        pause = true;
        if (i1 < l1 - 1)
          i1++;
        else {
          POCs.append(POC);
          break;
        }
        i2 = 0;
      }
    }
    int max_POC = POCs[0];
    for (auto POC : POCs)
      if (POC > max_POC)
        max_POC = POC;
    std::cout << "\tCalculated: max_POC = " << max_POC << std::endl;
    std::cout << "\tCalculated: max_POC / (l1 + l2) = "
              << float(max_POC) / (l1 + l2) << std::endl;
    return float(max_POC) / (l1 + l2);
  }
  
  /*!
   * @fn StringSearch::get_POC2
   */
  static float get_POC2(const QString &e1, const QString &e2) {
    std::cout << "--------" << std::endl;
    std::cout << "\tfn get_POC2" << std::endl;
    std::cout << "\tGot e1 as \"" << e1.toStdString() << "\" and e2 as \"" << e2.toStdString() << "\"" << std::endl;
    if (e1.length() + e2.length() == 0)
      return 0.0;
    QList<int> indices;
    int i = 0;
    for (int j = 0; j < e2.length(); j++)
      if ((i = e1.indexOf(e2[j], j)) != -1 and i <= j + (e1.length() - e2.length()))
        indices.append(i);
    std::cout << "\tCalculated indices.length() = " << indices.length() << std::endl;
    if (not indices.length())
      return 0.0;
    QList<QList<int>> subs;
    std::cout << "\tCalculated indices:" << std::endl;
    for (auto i : indices) {
      std::cout << "\t  " << i << std::endl;
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
  static QPair<int, int> contains(QString that, QString inner,
                                  float EL = 0.66) {
    std::cout << "--------" << std::endl;
    std::cout << "\tfn contains" << std::endl;
    std::cout << "\tGot that as \"" << that.toStdString()
              << "\" and inner as \"" << inner.toStdString() << "\""
              << std::endl;
    QList<WordMetadata> that_metadata, inner_metadata;
    auto p1 = purify(that), p2 = purify(inner);
    if (p1.length() < p2.length()) {
      auto x = p1;
      p1 = p2;
      p2 = x;
    }
    std::cout << "\tCalculated p1 as \"" << p1.toStdString()
              << "\" and p2 as \"" << p2.toStdString() << "\"" << std::endl;
    //     for (auto str : {p1, p2}) {
    int last = 0;
    for (auto word : p1.split(" ")) {
      WordMetadata wmd;
      wmd.word = word;
      wmd.i1 = p1.indexOf(word, last);
      wmd.i2 = wmd.i1 + word.length();
      last = wmd.i2;
      that_metadata.append(wmd);
      std::cout << "\t  Calculated that WordMetadata:" << std::endl;
      std::cout << "\t    wmd.word = \"" << wmd.word.toStdString() << "\""
                << std::endl;
      std::cout << "\t    wmd.i1 = " << wmd.i1 << std::endl;
      std::cout << "\t    wmd.i2 = " << wmd.i2 << std::endl;
    }
    last = 0;
    for (auto word : p2.split(" ")) {
      WordMetadata wmd;
      wmd.word = word;
      wmd.i1 = p2.indexOf(word, last);
      wmd.i2 = wmd.i1 + word.length();
      last = wmd.i2;
      inner_metadata.append(wmd);
      std::cout << "\t  Calculated inner WordMetadata:" << std::endl;
      std::cout << "\t    wmd.word = \"" << wmd.word.toStdString() << "\""
                << std::endl;
      std::cout << "\t    wmd.i1 = " << wmd.i1 << std::endl;
      std::cout << "\t    wmd.i2 = " << wmd.i2 << std::endl;
    }
    //     }
    std::cout << "\tCalculated inner_metadata.length() = "
              << inner_metadata.length() << std::endl;
    if (inner_metadata.length() == 0)
      return QPair<int, int>(0, 0);
    // TODO Этот алгоритм не шибко оптимален по скорости, но вполне
    // работоспособен.
    // FIXME Алгоритм не гарантирует сопоставимость по количеству выбранных
    // слов. Он вполне сработает с одним POC на два слова из inner.
    QList<QPair<float, WordMetadata>> common;
    for (auto w2 : inner_metadata) {
      QPair<float, WordMetadata> max_POC = {0.0, WordMetadata()};
      // TODO Можно ещё сделать реализацию POC по синонимам.
      for (auto w1 : that_metadata) {
        float POC = get_POC2(w1.word, w2.word);
        std::cout << "\tCalculated POC = " << POC << " when EL = " << EL
                  << " and max_POC = " << max_POC.first << std::endl;
        if (POC >= EL and POC > max_POC.first)
          max_POC = QPair<float, WordMetadata>(POC, w1);
      }
      common.append(max_POC);
    }
    std::cout << "\tCalculated common.length() = " << common.length()
              << std::endl;
    if (common.length() / inner_metadata.length() >= EL) {
      int min_i = that.length() - 1, max_i = 0;
      for (auto wmd_pair : common) {
        if (wmd_pair.second.i1 < min_i)
          min_i = wmd_pair.second.i1;
        if (wmd_pair.second.i2 > max_i)
          max_i = wmd_pair.second.i2;
      }
      std::cout << "\tCalculated min_i = " << min_i << " and max_i = " << max_i
                << std::endl;
      return QPair<int, int>(min_i, max_i);
    } else
      return QPair<int, int>(0, 0);
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
  static QString replace(QString that, QPair<int, int> indices,
                         QString to = "") {
    std::cout << "--------" << std::endl;
    std::cout << "\tfn replace" << std::endl;
    std::cout << "\tGot that as \"" << that.toStdString()
              << "\" with len = " << that.length()
              << " and indices: " << indices.first << " and " << indices.second
              << std::endl;
    auto k = that.left(indices.first);
    auto l = that.right(that.length() - indices.second - 1);
    std::cout << "\tCalculated k as \"" << k.toStdString() << "\" and l as \""
              << l.toStdString() << "\"" << std::endl;
    return that.left(indices.first) + to +
           that.right(that.length() - indices.second - 1);
  }
};

#endif
