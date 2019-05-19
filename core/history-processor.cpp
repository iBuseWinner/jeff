#include "history-processor.h"
#include <QDebug>

/*
 * All short named variables and their explanations:
 * {Meths} <- core methods
 * {mh}{_mh} <- message history
 * {sf} <- savefile
 */

/*!
 * Arguments: settings {*_settings} [reference to Settings instance],
 *            QObject {*parent}.
 * Sets references to private objects.
 */
historyProcessor::historyProcessor(CoreMethods *_Meths, QObject *parent)
    : QObject(parent) {
  Meths = _Meths;
}

/*!
 * Argument: QString {filename} [filename to save].
 * Saves {mh} to file.
 */
void historyProcessor::save(const QString &filename) {
  Meths->writeMessageHistory(mh, new QFile(filename));
}

/*!
 * Argument: QString {filename} [filename to load].
 * Loads {mh} from file.
 */
void historyProcessor::load(const QString &filename) {
  merge(Meths->readMessageHistory(new QFile(filename)));
  emit sendMessageHistory(mh);
}

/*!
 * Argument: message {_message}.
 * Removes {_message} from {mh}.
 */
void historyProcessor::removeOne(message _message) {
  // Qt does not provide the ability to compare structures, therefore
  // historyProcessor independently compares elements and deletes an identical
  // one.
  // Everything else, there is no need to compare all the characteristics.
  // Authorship and the date of creation during the operation of the program
  // will be able to coincide only once.
  for (int i = 0; i < mh.length(); i++)
    if ((mh.at(i).aType == _message.aType) &&
        (mh.at(i).datetime == _message.datetime)) {
      mh.removeAt(i);
      break;
    }
}

/*!
 * Argument: QList of messages {_mh} [loaded message history].
 * Combines the current message history with the loaded history.
 */
void historyProcessor::merge(QList<message> _mh) {
  if (_mh.length() < 1) return;
  mh.append(_mh);
  mh = sorting(mh);
}

/*!
 * Argument: QList of messages {unsorted} [unsorted history].
 * Line up messages in chronological order.
 * Algorithm: insertion sort.
 * Returns: sorted QList of messages.
 */
QList<message> historyProcessor::sorting(QList<message> unsorted) {
  if (unsorted.length() <= 1) return unsorted;
  message section = unsorted.takeAt(int(unsorted.length() / 2));
  QList<message> earlierMessages;
  QList<message> laterMessages;
  for (auto shadow : unsorted) {
    if (shadow.datetime > section.datetime)
      laterMessages.append(shadow);
    else
      earlierMessages.append(shadow);
  }
  QList<message> sorted;
  earlierMessages = sorting(earlierMessages);
  for (auto shadow : earlierMessages) sorted.append(shadow);
  sorted.append(section);
  laterMessages = sorting(laterMessages);
  for (auto shadow : laterMessages) sorted.append(shadow);
  return sorted;
}
