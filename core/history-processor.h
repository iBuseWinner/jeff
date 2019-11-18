#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include <QFileDialog>
#include <QList>
#include <QStringList>
#include "core/core-methods.h"
#include "widgets/a_message.h"

/*!
 * Class: historyProcessor.
 * Stores, exports and loads message history.
 */
class historyProcessor : public QObject {
  Q_OBJECT
 public:
  // Functions:
  historyProcessor(CoreMethods *_Meths, QObject *parent = nullptr);
  void save(const QString &filename);
  void load(const QString &filename);
  void removeOne(message _message);

  /*! Adds a message to the story. */
  void append(const message &_message) { mh.append(_message); }
  /*! Clears the history. */
  void clear() { mh.clear(); }

 signals:
  QList<message> sendMessageHistory(QList<message> _mh);

 private:
  // Objects:
  CoreMethods *Meths = nullptr;
  QList<message> mh;
};

#endif  // HISTORY_PROCESSOR_H
