#ifndef CACHE_H
#define CACHE_H

#include "core-kit/model/nlp/expression.hpp"

/*! @typedef Cache
 *  @brief Contains cached response expressions.  */
typedef QList<Expression> Cache;

/*! @typedef CacheWithIndices
 *  @brief Contains cached response expressions and their indexes in user input.  */
typedef QList<ExpressionCoverage> CoverageCache;

#endif
