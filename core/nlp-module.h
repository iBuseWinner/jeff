#ifndef NLPMODULE_H
#define NLPMODULE_H

#include "core/basis.h"
#include "core/database/sqlite.h"
#include "model/expression.h"
#include "model/nlp/cache.h"
#include "model/nlp/responsewo.h"
#include "standard-templates.h"
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>

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
    _settings_path = _basis->get_settings_path();
    load_cache();
  }

  /*!
   * @fn NLPmodule::~NLPmodule
   * @brief Saves cache.
   */
  ~NLPmodule() { save_cache(); }

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
  QString _settings_path;

  // Constants:
  inline static const QString cache_path = "";

  // Functions described in `nlp-module.cpp`:
  Cache select_from_cache(const QString &input);
  Cache select_from_sql(const QString &input);
};

#endif // NLPMODULE_H
