#include "cacher.hpp"

/*! @brief The constructor. */
Cacher::Cacher(QObject *parent) : QObject(parent) { cache = new Cache(); }
/*! @brief The destructor. */
Cacher::~Cacher() { delete cache; }
/*! @brief Adds multiple expressions to @a cache. */
void Cacher::append(Cache _cache) { for (auto expr : _cache) append(expr); }
/*! @brief Returns a copy of the cache. */
const Cache Cacher::get() { return *cache; }
/*! @brief Returns the pointer to the cache. */
Cache *Cacher::get_ptr() { return cache; }

/*! @brief Adds an expression to @a cache, unless there is already an expression identical to it in the cache.  */
void Cacher::append(Expression expression) {
  if (not cache->contains(expression)) cache->append(expression);
  else {
    auto i = cache->indexOf(expression);
    (*cache)[i].properties = expression.properties;
    (*cache)[i].use_cases = expression.use_cases;
  }
}
