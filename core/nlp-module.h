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
public:
  // Functions:
  /*!
   * @fn NLPmodule::NLPmodule
   * @brief The constructor.
   * @param[in,out] basis reference to the Basis instance
   * @param[in,out] parent QObject parent
   */
  NLPmodule(Basis *basis, PythonModule *pm, QObject *parent = nullptr) 
    : QObject(parent), _basis(basis), _pm(pm) {
    _gen = new QRandomGenerator(QTime::currentTime().msec());
    load_cache();
  }

  /*!
   * @fn NLPmodule::~NLPmodule
   * @brief Saves cache.
   */
  ~NLPmodule() {
    save_cache();
    delete _gen;
  }

  // Functions described in `nlp-module.cpp`:
  void load_cache();
  void save_cache();
  void search_for_suggests(const QString &input);

signals:
  /*! @brief Sends a response expression to @a Core. */
  QString response(QString response);

private:
  // Objects:
  Basis *_basis = nullptr;
  PythonModule *_pm = nullptr;
  QRandomGenerator *_gen = nullptr;

  // Constants:
  inline static const QString cache_path = "";

  // Functions described in `nlp-module.cpp`:
  CacheWithIndices select_from_cache(const QString &input);
  CacheWithIndices select_from_db(const QString &input);

  CacheWithIndices select_candidates(CacheWithIndices selection, QString input);
  QPair<QString, QString> compose_answer(QString input, CacheWithIndices candidates);
};

#endif
