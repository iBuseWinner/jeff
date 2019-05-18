#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include <QFileDialog>
#include <QStringList>
#include "core/settings.h"
#include "widgets/a_message.h"

/*!
 * Class: historyProcessor.
 * Stores, exports and loads message history.
 */
class historyProcessor : public QObject {
  Q_OBJECT
 public:
  // Functions:
  historyProcessor(settings *_settings, QObject *parent = nullptr);
  void save(const QString &filename);
  void removeOne(message _message);
  QVector<message> load(const QString &filename);

  /*! Adds a message to the story. */
  void append(message _message) { mh.append(_message); }
  /*! Clears the history. */
  void clear() { mh.clear(); }

 private:
  // Objects:
  settings *st = nullptr;
  QVector<message> mh;
};

#endif  // HISTORY_PROCESSOR_H
