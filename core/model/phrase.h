#ifndef PHRASE_H
#define PHRASE_H

#include "core/model/nlp/options.h"
#include <QList>
#include <QJsonObject>
#include <QSet>
#include <QString>

/*!
 * @class Phrase
 * @brief Contains information about a representation of a sentence in SQL database.
 */
class Phrase {
public:
  // Objects:
  /*! Phrase address in the database table. */
  int address = -1;
  /*! Phrase text. */
  QString expression = "";
  /*! Links to other phrases (reagents). */
  QSet<int> links;
  /*! If this phrase is a reagent, indicates whether a script needs to be run. */
  bool exec = false;
  /*! Additional phrase properties stored in the database. */
  Options properties;
  
  // Constructors:
  Phrase() {}
  Phrase(const QJsonObject &json_object) {
    address = json_object["address"].toInt();
    expression = json_object["expression"].toString();
    links = unpack_links(json_object["links"].toString());
    exec = json_object["exec"].toBool();
    properties = parse_props(json_object["properties"]);
  }
  
  // Functions:
  /*! @brief Compares two phrases.  */
  friend bool operator==(Phrase p1, Phrase p2) {
    return p1.expression == p2.expression and p1.exec == p2.exec;
  }
  
  /*! @brief Turns @a phrase into a JSON object. */
  QJsonObject to_json() {
    return {{"address", address},
            {"expression", expression},
            {"links", pack_links(links)},
            {"exec", exec},
            {"properties", pack_props(properties)}};
  }
  
  /*! @brief Properties' parser. */
  static Options parse_props(QJsonValue _aps) {
    Options aps;
    for (auto _ap_key : _aps.toObject().keys()) aps[_ap_key] = _aps[_ap_key].toString();
    return aps;
  }
  
  /*! @brief Properties' packer. */
  static QJsonObject pack_props(Options aps) {
    QJsonObject _aps;
    for (auto ap_key : aps.keys()) _aps.insert(ap_key, aps[ap_key]);
    return _aps;
  }
  
  /*! @brief Unpacks references from a string into a set of indices. */
  static QSet<int> unpack_links(const QString &links) {
    QSet<int> unpacked;
    if (links.isEmpty()) return QSet<int>();
    QStringList splitted = links.split(",");
    for (auto link : splitted) unpacked.insert(link.toInt());
    return unpacked;
  }
  
  /*! @brief Packs references from a set to a string. */
  static QString pack_links(const QSet<int> &links) {
    QString packed;
    for (auto link : links) packed.append(QString::number(link) + ",");
    packed.truncate(packed.length() - 1);
    return packed;
  }
};

/*!
 * @typedef Phrases
 * @brief Contains list of phrases.
 */
typedef QList<Phrase> Phrases;

#endif
