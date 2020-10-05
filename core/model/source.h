#ifndef SOURCE_H
#define SOURCE_H

#include <QJsonObject>
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

  /*!
   * @fn to_json
   * @brief Turns @a source into a JSON object.
   * @param[in] source source parameters
   * @returns converted properties of @a source
   */
  friend QJsonObject to_json(const Source &source) {
    return {{"container", source.table_name},
            {"disabled", source.is_disabled},
            {"path", source.path},
            {"title", source.table_title}};
  }
};

/*!
 * @fn to_source
 * @brief Turns @a json_object into a source.
 * @param[in] json_object source in JSON
 * @returns source properties
 */
Source to_source(const QJsonObject &json_object) {
  return {json_object["path"].toString(), json_object["container"].toString(),
          json_object["title"].toString(), json_object["disabled"].toBool()};
}

#endif // SOURCE_H
