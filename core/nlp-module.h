#ifndef NLPMODULE_H
#define NLPMODULE_H

#include "core/basis.h"
#include "core/database/sqlite.h"
#include "core/model/expression.h"
#include "core/model/nlp/cache.h"
#include "core/model/nlp/responsewo.h"
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
#include <iostream>

/*!
 * @class NLPmodule
 * @brief Looks for regular expressions in user input and displays answers to
 * them.
 * @sa Basis
 */
class NLPmodule : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*!
   * @fn NLPmodule::NLPmodule
   * @brief The constructor.
   * @param[in,out] basis reference to the Basis instance
   * @param[in,out] parent QObject parent
   */
  NLPmodule(Basis *basis, QObject *parent = nullptr)
      : QObject(parent), _basis(basis) {
    _gen = new QRandomGenerator(QTime::currentTime().msec());
    load_cache();
  }

  /*!
   * @fn NLPmodule::~NLPmodule
   * @brief Saves cache.
   */
  ~NLPmodule() {
    save_cache();
    for (auto *expr : _cache)
      delete expr;
  }

  // Functions described in `nlp-module.cpp`:
  void load_cache();
  void save_cache();
  void search_for_suggests(const QString &input);

signals:
  /*!
   * @brief Sends a response expression to @a Core.
   * @sa Core
   */
  QString response(QString response);

  /*!
   * @brief Sends a response expression with additional options for @a Core and
   * others.
   * @sa Core
   */
  ResponseWO response_wo(ResponseWO response_wo);

private:
  // Objects:
  Basis *_basis = nullptr;
  Cache _cache;
  QRandomGenerator *_gen = nullptr;

  // Constants:
  inline static const QString cache_path = "";

  // Functions described in `nlp-module.cpp`:
  Cache select_from_cache(const QString &input);
  Cache select_from_db(const QString &input);
  
  QMap<int, Expression *> select_candidates(Cache selection, QString input);
  QPair<QString, QString> compose_answer(QString input,
                                         QMap<int, Expression *> candidates);
};

#endif
