#include "cacher.h"

/*!
 * @fn Cacher::Cacher
 * @brief The constructor.
 */
Cacher::Cacher(QObject *parent) : QObject(parent) { cache = new Cache(); }

/*!
 * @fn Cacher::~Cacher
 * @brief The destructor.
 */
Cacher::~Cacher() { delete cache; }

/*!
 * @fn Cacher::append
 * @brief Adds an expression to @a cache, unless there is already an expression identical to it 
 * in the cache.
 * @param[in] expression to be added in @a cache
 */
void Cacher::append(Expression expression) {
  cache_mutex.lock();
  if (not cache->contains(expression)) cache->append(expression);
  cache_mutex.unlock();
}

/*!
 * @fn Cacher::append
 * @brief Adds multiple expressions to @a cache.
 * @param[in] _cache to be added to @a cache
 * @warning This function not checks for the presence of an expression in the cache, 
 * unlike {void Cacher::append(Expresson expression)}.
 */
void Cacher::append(Cache _cache) { cache->append(_cache); }

/*!
 * @fn Cacher::get
 * @brief Returns a copy of the cache.
 */
const Cache Cacher::get() { return *cache; }
