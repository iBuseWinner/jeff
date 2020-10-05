#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "source.h"
#include "nlp/options.h"
#include <QList>
#include <QMap>
#include <QString>

/*!
 * @struct Expression
 * @brief Contains information about a single expression of NLPmodule.
 */
struct Expression {
  /*! What we looking for. */
  QString activator_text;
  /*! Answer expression. */
  QString reagent_text;
  /*! Reagents properties (name, value). */
  Options properties;
  /*! How much times this expression used. */
  ushort use_cases = 0;
};

#endif // EXPRESSION_H
