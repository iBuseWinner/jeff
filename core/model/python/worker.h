#ifndef PYTHON_WORKER_H
#define PYTHON_WORKER_H

#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include "core/basis.h"
#include "core/history-processor.h"
#include "core/model/expression.h"
#include "core/model/nlp/cache.h"
#include "core/model/python/object.h"
#include "core/model/python/script.h"

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

/*! @class PythonWorker
 *  @brief Contains the logic of working with Python scripts.  */
class PythonWorker : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(PythonWorker)
public:
  // Functions described in `worker.cpp`:
  PythonWorker(Basis *_basis = nullptr, HProcessor *_hp = nullptr, QObject *parent = nullptr);
  ~PythonWorker();
  QJsonObject request_answer(
    ReactScript *script, const Expression &expression, const QString &user_expression
  );
  QJsonObject request_scan(CustomScanScript *script, const QString &user_expression);
  QJsonObject request_compose(
    CustomComposeScript *script, const QString &user_expression, CacheWithIndices sorted
  );
  
signals:
  /*! @brief Notifies of a script error. */
  QString script_exception(QString error);
  
private:
  // Objects:
  Basis *basis = nullptr;
  HProcessor *hp = nullptr;
  QString _current_path;
  
  // Functions described in `worker.cpp`:
  QJsonObject run(QString path, QString def_name, QJsonObject transport);
};

#endif

