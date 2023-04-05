#ifndef CACHER_H
#define CACHER_H

#include "core-kit/model/nlp/cache.hpp"
#include <QMutex>
#include <QObject>

/*! @class Cacher
 *  @brief Manages the expression cache. */
class Cacher : public QObject {
  Q_OBJECT
public:
  // Functions described in `cacher.cpp`:
  Cacher(QObject *parent = nullptr);
  ~Cacher();
  void append(Expression expression);
  void append(Cache _cache);
  const Cache get();
  Cache *get_ptr();

private:
  // Objects:
  Cache *cache = nullptr;
};

#endif
