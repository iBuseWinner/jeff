#ifndef SCRIPT_H
#define SCRIPT_H

#include "yelloger.h"
#include <QFile>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>

/*! @enum ScriptType
 *  @brief Specifies the types of scripts that are used in Jeff.
 *  @details There are the following types of scripts:
 *  0. React - Reacts to user expressions, added directly to databases
 *  1. CustomScan - Scans and responds to user input, replacing Jeff's NLPmodule
 *  2. CustomCompose - Receives response options for user input from Jeff and composes the response  */
enum ScriptType { NoneScript, React, CustomScan, CustomCompose };

/*! @class ScriptMeta
 *  @brief Contains metadata about a script.  */
class ScriptMeta {
public:
  // Functions described in `script.cpp`:
  ScriptMeta();
  ScriptMeta(const QJsonObject &json_object);
  QJsonObject to_json() const;
  QString to_string() const;
  static ScriptMeta *from_string(QString string);
  static ScriptMeta *from_origin(const QString &origin);

  // Objects:
  /*! @brief inner props */
  bool valid = false;
  bool is_for_embedded_python = true;
  QString origin;
  /*! @brief props from file */
  ScriptType stype = NoneScript;
  QString filepath;
  QString fn_name;
  QString working_dir;
  QString program;
  QMap<QString, QString> envs;
  QStringList args;
  QStringList required_memory_cells;
  bool required_user_input = false;
  int required_history_parts = 0;
  bool required_adprops = false;
};

/*! @brief Container for scripts. */
typedef QList<ScriptMeta *> ScriptsMeta;

#endif
