#ifndef PYTHON_WORKER_H
#define PYTHON_WORKER_H

#include "core-kit/basis.hpp"
#include "core-kit/history-processor.hpp"
#include "core-kit/extensions/script.hpp"
#include "core-kit/model/nlp/cache.hpp"
#include "core-kit/model/nlp/expression.hpp"
#include "core-kit/model/python/object.hpp"
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>

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
  QJsonObject request_answer(ScriptMeta *script, const Expression &expression, const QString &input);
  QJsonObject request_scan(ScriptMeta *script, const QString &user_expression);
  QJsonObject request_compose(ScriptMeta *script, const QString &user_expression, CacheWithIndices sorted);
  
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

