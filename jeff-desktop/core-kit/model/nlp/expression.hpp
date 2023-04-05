#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "core-kit/model/nlp/options.hpp"
#include "core-kit/model/phrase.hpp"
#include "core-kit/model/source.hpp"
#include <QList>
#include <QMap>
#include <QString>

/*! @class Expression
 *  @brief Contains information about a single expression of JCK module.  */
class Expression {
public:
  // Objects:
  /*! What we looking for. */
  QString activator_text = "";
  /*! Answer expression. */
  QString reagent_text = "";
  /*! Reagents properties (name, value). */
  Options properties = Phrase::parse_props(QJsonObject());
  /*! How much times this expression used. */
  ushort use_cases = 0;
  /*! Is the reagent a script to be executed? */
  bool exec = false;

  // Constructors:
  Expression() {}
  Expression(const QJsonObject &json_object) {
    activator_text = json_object["activator_text"].toString();
    reagent_text = json_object["reagent_text"].toString();
    properties = Phrase::parse_props(json_object["properties"].toObject());
    use_cases = json_object["use_cases"].toInt();
    exec = json_object["exec"].toBool();
  }

  // Functions:
  /*! @brief Compares two expressions. */
  friend bool operator==(Expression e1, Expression e2) {
    return e1.activator_text == e2.activator_text and
           e1.reagent_text == e2.reagent_text and
           e1.exec == e2.exec and
           e1.properties == e2.properties;
  }

  /*! @brief Turns @an expresson into a JSON object. */
  QJsonObject to_json() const {
    return {{"activator_text", activator_text},
            {"reagent_text", reagent_text},
            {"properties", Phrase::pack_props(properties)},
            {"use_cases", use_cases},
            {"exec", exec}};
  }
  
  /*! @brief Translates @a object into an expression. */
  static Expression from_object(const QJsonObject &object, bool *err = nullptr) {
    auto expression = Expression(object);
    if (expression.activator_text.isEmpty() or expression.reagent_text.isEmpty()) {
      Yellog::Error("It's impossible to parse expression from JSON.");
      if (err) *err = true;
      return Expression();
    }
    return expression;
  }

  /*! @brief Returns @a weight of expression. */
  int weight() { return properties.value("weight").toInt(); }
  /*! @brief Tells whether the expression can be answered together with other expressions. */
  bool consonant() { return bool(properties.value("consonant").toInt()); }
};

/*! @struct ExpressionCoverage
 *  @brief Contains the expression, its percentage of user input coverage, and the indices of the occurrence.  */
struct ExpressionCoverage {
  Expression expression;
  QMap<int, int> coverage_indices;
  float total_POC = 0.0;
};

typedef QList<Expression> Expressions;

#endif
