#ifndef SOURCESTRUCT_H
#define SOURCESTRUCT_H

#include <QString>

/*!
 * @struct Source
 * @brief Contains data about the source of regular expressions for the
 * NLPmodule.
 */
struct Source {
  /*! Path to database. */
  QString path = "";
  /*! Table name [UUID, name of database table]. */
  QString table_name = "";
  /*! Table title [username]. */
  QString table_title = "";
  /*! Means that does not participate in the selection of reagents. */
  bool is_disabled = false;
  /*! Means that not available for adding expressions from ASW. */
  bool is_read_only = false;
  /*! Means that unavailable until purposeful entry. */
  bool is_private = false;
  /*! Means that the search for reagents occurs exclusively in it. */
  bool is_catching = false;
  /*! Means that the input is first validated using this container. */
  bool is_prioritised = false;

  /*! Compares two sources. They are identical if they have the same table name
   * and are in the same database. */
  friend bool operator==(Source s1, Source s2) {
    return s1.table_name == s2.table_name and s1.path == s2.path;
  }
};

#endif // SOURCESTRUCT_H
