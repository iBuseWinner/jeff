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
HProcessor::HProcessor(Basis *_basis, QObject *parent)
    : QObject(parent) {
  basis = _basis;
}

/*!
 * Argument: QString {filename} [filename to save].
 * Saves {mh} to file.
 */
void HProcessor::save(const QString &filename) {
  basis->writeMessageHistory(mh, new QFile(filename));
}

/*!
 * Argument: QString {filename} [filename to load].
 * Loads {mh} from file.
 */
void HProcessor::load(const QString &filename) {
  mh = basis->readMessageHistory(new QFile(filename)) + mh;
  emit sendMessageHistory(mh);
}

/*!
 * Argument: message {_message}.
 * Removes {_message} from {mh}.
 */
void HProcessor::removeOne(message _message) {
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
