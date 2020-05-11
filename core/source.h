#ifndef CONTAINERSSTRUCT_H
#define CONTAINERSSTRUCT_H

#include <QString>

/*!
 * Struct: Source.
 * Contains data about the container of regular expressions for the NLP-module.
 */
struct Source {
  /*! Path to database. */
  QString path = "";
  /*! Table name [UUID, name of database table]. */
  QString tableName = "";
  /*! Table title [username]. */
  QString tableTitle = "";
  /*! Means that does not participate in the selection of reagents. */
  bool isDisabled = false;
  /*! Means that not available for adding expressions from ASW. */
  bool isReadOnly = false;
  /*! Means that unavailable until purposeful entry. */
  bool isPrivate = false;
  /*! Means that the search for reagents occurs exclusively in it. */
  bool isCatching = false;
  /*! Means that the input is first validated using this container. */
  bool isPrioritised = false;
};

#endif  // CONTAINERSSTRUCT_H
