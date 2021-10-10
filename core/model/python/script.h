#ifndef SCRIPT_H
#define SCRIPT_H

/*!
 * @struct ScriptMetadata
 * @brief Contains metadata about a script.
 */
struct ScriptMetadata {
  /*! The path to the script on disk. */
  QString path;
  /*! Does it have a function that should run every time Jeff starts? */
  bool startup;
  /*! Does it have a function that processes all user input and performs certain
   * actions? */
  bool custom_scan;
  /*! Does it have a function that is performed only when a certain semantic
   * load is found? */
  bool answer;
};

/*!
 * @typedef Scripts
 * @brief Contains metadata about multiple scripts.
 */
typedef QList<ScriptMetadata> Scripts;

#endif
