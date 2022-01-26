#ifndef CACHER_H
#define CACHER_H

#include "core/model/nlp/cache.h"
#include <QMutex>
#include <QObject>

class Cacher : public QObject {
  Q_OBJECT
public:
  // Functions described in `cacher.cpp`:
  Cacher(QObject *parent = nullptr);
  ~Cacher();
  void append(Expression expression);
  void append(Cache _cache);
  const Cache get();

private:
  // Objects:
  QMutex cache_mutex;
  Cache *cache = nullptr;
};

#endif
