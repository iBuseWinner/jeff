#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "nlp/options.h"
#include "source.h"
#include <QList>
#include <QMap>
#include <QString>

/*!
 * @class Expression
 * @brief Contains information about a single expression of NLPmodule.
 */
class Expression {
public:
  /*! Constructors. */
  Expression() {}
  Expression(const QJsonObject &json_object) {
    activator_text = json_object["activator_text"].toString();
    reagent_text = json_object["reagent_text"].toString();
    properties = parse_props(json_object["aps"]);
    use_cases = json_object["use_cases"].toInt();
    exec = json_object["exec"].toBool();
  }
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
  /*!
   * @fn Expression::to_json
   * @brief Turns @an expresson into a JSON object.
   * @param[in] expression
   * @returns converted properties of @an expression
   */
  QJsonObject to_json() {
    return {{"activator_text", activator_text},
            {"reagent_text", reagent_text},
            {"properties", pack_props(properties)},
            {"use_cases", use_cases},
            {"exec", exec}};
  }

  int weight() {
    if (properties.contains("weight")) return properties.value("weight").toInt();
    else return 0;
  }

private:
  /*! Properties' parser. */
  Options parse_props(QJsonValue _aps) {
    Options aps;
    for (auto _ap_key : _aps.toObject().keys()) aps[_ap_key] = _aps[_ap_key].toString();
    return aps;
  }
  /*! Properties' packer. */
  QJsonObject pack_props(Options aps) {
    QJsonObject _aps;
    for (auto ap_key : aps.keys()) _aps.insert(ap_key, aps[ap_key]);
    return _aps;
  }
};

typedef QPair<QMap<int, int>, Expression *> ExpressionWithIndices;

#endif
