#ifndef NLPMODULE_H
#define NLPMODULE_H

#include "core/basis.h"
#include "core/python-module.h"
#include "core/database/sqlite.h"
#include "core/model/expression.h"
#include "core/model/nlp/cache.h"
#include "core/model/nlp/cacher.h"
#include "core/model/nlp/stringssearch.h"
#include "core/standard-templates.h"
#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>
#include <future>

/*!
 * @class NLPmodule
 * @brief Looks for regular expressions in user input and displays answers to them.
 * @sa Basis
 */
class NLPmodule : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(NLPmodule)
public:
  // Functions described in `nlp-module.cpp`:
  NLPmodule(Basis *_basis, PythonModule *_pm, HProcessor *_hp, QObject *parent = nullptr);
  ~NLPmodule();
  void load_cache();
  void save_cache();
  void search_for_suggests(const QString &input);

signals:
  /*! @brief Sends a response expression to @a Core. */
  QString response(QString response);

private:
  // Objects:
  Basis *basis = nullptr;
  PythonModule *pm = nullptr;
  HProcessor *hp = nullptr;
  QRandomGenerator *gen = nullptr;

  // Constants:
  inline static const QString cache_path = "";

  // Functions described in `nlp-module.cpp`:
  CacheWithIndices select_from_cache(const QString &input);
  CacheWithIndices select_from_db(const QString &input);
  CacheWithIndices select_candidates(CacheWithIndices selection, QString input);
  QPair<QString, QString> compose_answer(QString input, CacheWithIndices candidates);
};

#endif
