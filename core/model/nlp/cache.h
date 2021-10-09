#ifndef CACHE_H
#define CACHE_H

#include "../expression.h"

/*!
 * @typedef Cache
 * @brief Contains cached response expressions.
 */
typedef QList<Expression *> Cache;

/*!
 * @typedef CacheWithIndices
 * @brief Contains cached response expressions and their indexes in user input.
 */
typedef QMap<int, ExpressionWithIndices> CacheWithIndices;

#endif
