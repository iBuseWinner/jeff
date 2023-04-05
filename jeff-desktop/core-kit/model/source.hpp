#ifndef SOURCE_H
#define SOURCE_H

#include <QJsonObject>
#include <QString>

/*! @class Source
 *  @brief Contains data about a source of phrases for the JCK.  */
class Source {
public:
  // Objects:
  /*! Path to database. */
  QString path = "";
  /*! Table name [UUID, name of database table]. */
  QString table_name = "";
  /*! Table title [username]. */
  QString table_title = "";
  /*! Means that it doesn't participate in choosing of reagents. */
  bool is_disabled = false;
  /*! If a given source has a higher weight than others, then if a reagent is found in it, it will be used with much higher priority. */
  int weight = 0;
  
  // Constructors:
  Source() {}
  Source(const QJsonObject &json_object) {
    path = json_object["path"].toString();
    table_name = json_object["container"].toString();
    table_title = json_object["title"].toString();
    is_disabled = json_object["disabled"].toBool();
    weight = json_object["weight"].toInt();
  }
  
  // Functions:
  /*! @brief Compares two sources. They are identical if they have the same table name
   *  and are in the same database.  */
  friend bool operator==(Source s1, Source s2) {
    return s1.table_name == s2.table_name and s1.path == s2.path;
  }
  
  /*! @brief Turns @a source into a JSON object. */
  QJsonObject to_json() const {
    return {{"container", table_name},
            {"disabled", is_disabled},
            {"path", path},
            {"title", table_title},
            {"weight", weight}};
  }
};

typedef QList<Source> Sources;

#endif
