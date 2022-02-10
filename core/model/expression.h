#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "core/model/nlp/options.h"
#include "core/model/source.h"
#include <QList>
#include <QMap>
#include <QString>

/*!
 * @class Expression
 * @brief Contains information about a single expression of NLPmodule.
 */
class Expression {
public:
  // Objects:
  /*! What we looking for. */
  QString activator_text;
  /*! Answer expression. */
  QString reagent_text;
  /*! Reagents properties (name, value). */
  Options properties;
  /*! How much times this expression used. */
  ushort use_cases = 0;
  /*! Is the reagent a script to be executed? */
  bool exec = false;
  
  // Constructors:
  Expression() {}
  Expression(const QJsonObject &json_object) {
    activator_text = json_object["activator_text"].toString();
    reagent_text = json_object["reagent_text"].toString();
    properties = parse_props(json_object["properties"]);
    use_cases = json_object["use_cases"].toInt();
    exec = json_object["exec"].toBool();
  }
  
  // Functions:
  /*! @brief Compares two expressions. */
  friend bool operator==(Expression e1, Expression e2) {
    return e1.activator_text == e2.activator_text and
           e1.reagent_text == e2.reagent_text and
           e1.exec == e2.exec;
  }
  
  /*! @brief Turns @an expresson into a JSON object. */
  QJsonObject to_json() {
    return {{"activator_text", activator_text},
            {"reagent_text", reagent_text},
            {"properties", pack_props(properties)},
            {"use_cases", use_cases},
            {"exec", exec}};
  }

  /*! @brief Returns @a weight of expression. */
  int weight() { return properties.value("weight").toInt(); }
  /*! @brief Tells whether the expression can be answered together with other expressions. */
  bool consonant() { return bool(properties.value("consonant").toInt()); }

  /*! @brief Properties' parser. */
  static Options parse_props(QJsonValue _aps) {
    Options aps;
    for (auto _ap_key : _aps.toObject().keys()) aps[_ap_key] = _aps[_ap_key].toString();
    return aps;
  }
  
  /*! @brief Properties' packer. */
  static QJsonObject pack_props(Options aps) {
    QJsonObject _aps;
    for (auto ap_key : aps.keys()) _aps.insert(ap_key, aps[ap_key]);
    return _aps;
  }
};

typedef QList<Expression> Expressions;

typedef QPair<QMap<int, int>, Expression> ExpressionWithIndices;

#endif
