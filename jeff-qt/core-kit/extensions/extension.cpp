#include "extension.hpp"

/*! @brief The constructors. */
ExtensionMeta::ExtensionMeta() {}
ExtensionMeta::ExtensionMeta(const QJsonObject &json_object) {
  if (not (
    json_object.contains("name")    and
    json_object.contains("title")   and
    json_object.contains("desc")    and
    json_object.contains("license") and
    json_object.contains("program")
  )) return;
  name    = json_object["name"]   .toString();
  title   = json_object["title"]  .toString();
  desc    = json_object["desc"]   .toString();
  license = json_object["license"].toString();
  program = json_object["program"].toString();
  if (json_object["authors"].isArray()) {
    QJsonArray authors_arr = json_object["authors"].toArray();
    for (auto v : authors_arr) {
      auto obj = v.toObject();
      QString author = obj["author"].toString();
      QMap<QString, QString> contacts;
      if (obj["contacts"].isObject()) {
        auto contacts_obj = obj["contacts"].toObject();
        for (auto key : contacts_obj.keys()) contacts[key] = contacts_obj[key].toString();
      }
      authors[author] = contacts;
    }
  }
  if (json_object["links"].isArray()) {
    auto links_arr = json_object["links"].toArray();
    for (auto link : links_arr) links.append(link.toString());
  }
  if (json_object.contains("working_dir")) working_dir = json_object["working_dir"].toString();
  if (json_object["envs"].isObject()) {
    auto envs_obj = json_object["envs"].toObject();
    for (auto key : envs_obj.keys()) {
      envs[key] = envs_obj[key].toString();
    }
  }
  if (json_object["args"].isArray()) {
    auto args_arr = json_object["args"].toArray();
    for (auto arg : args_arr) args.append(arg.toString());
  }
  if (json_object.contains("server_ip") and json_object.contains("server_port")) {
    is_server = true;
    server_addr = QHostAddress(json_object["server_addr"].toString());
    if (server_addr.isNull()) server_addr = QHostAddress("127.0.0.1");
    server_port = quint16(json_object["server_port"].toInt());
    if (json_object.contains("always_send")) {
      if (json_object["always_send"].toBool())
        always_send = true;
    }
  }
  valid = true;
}

/*! @brief Turns an extension metadata into a JSON object. */
QJsonObject ExtensionMeta::to_json() const {
  QJsonArray authors_arr;
  for (auto author : authors.keys()) {
    QJsonObject author_obj;
    QJsonObject contacts_obj;
    for (auto contact_type : authors[author].keys()) {
      contacts_obj[contact_type] = authors[author][contact_type];
    }
    author_obj["author"] = author;
    author_obj["contacts"] = contacts_obj;
    authors_arr.append(author_obj);
  }
  QJsonArray links_arr;
  for (auto link : links) links_arr.append(link);
  QJsonObject envs_obj;
  for (auto key : envs.keys()) envs_obj[key] = envs[key];
  QJsonArray args_arr;
  for (auto arg : args) args_arr.append(arg);
  QJsonObject result = {
    {"name", name},
    {"title", title},
    {"desc", desc},
    {"authors", authors_arr},
    {"license", license},
    {"links", links_arr},
    {"program", program},
    {"envs", envs_obj},
    {"args", args_arr}
  };
  if (is_server) {
    result["server_addr"] = server_addr.toString();
    result["server_port"] = int(server_port);
    if (always_send) result["always_send"] = always_send;
  }
  if (not working_dir.isEmpty()) result["working_dir"] = working_dir;
  return result;
}

/*! @brief Turns an extension metadata into a string. */
QString ExtensionMeta::to_string() const {
  QJsonDocument document(to_json());
  return QString::fromUtf8(document.toJson(QJsonDocument::Compact));
}

/*! @brief Translates @a string into an extension metadata. */
ExtensionMeta *ExtensionMeta::from_string(QString string) {
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(string.toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) {
    Yellog::Error("It's impossible to parse extension from JSON. Error int: %d", int(errors.error));
    return nullptr;
  }
  auto json_object = document.object();
  auto *extension_meta = new ExtensionMeta(json_object);
  if (not extension_meta) {
    Yellog::Error("Unable to make ExtensionMeta object from JSON.");
    return nullptr;
  }
  if (not extension_meta->valid) {
    Yellog::Error("Unable to make ExtensionMeta object from JSON.");
    delete extension_meta;
    return nullptr;
  }
  return extension_meta;
}

/*! @brief Reads extension's metadata from @a origin file. */
ExtensionMeta *ExtensionMeta::from_origin(const QString &origin, bool enabled) {
  Yellog::Trace("Given file: %s", origin.toStdString().c_str());
  QFile file(origin);
  if (not file.exists()) {
    Yellog::Error("File doesn't exist.");
    return nullptr;
  }
  if (not file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Yellog::Error("Unable to open file as RO & Text.");
    return nullptr;
  }
  QTextStream textStream(&file);
  auto text = textStream.readAll().toUtf8();
  file.close();
  auto *extension_meta = from_string(text);
  if (not extension_meta) {
    Yellog::Error("Unable to make ExtensionMeta object from origin.");
    return nullptr;
  }
  extension_meta->origin = origin;
  extension_meta->enabled = enabled;
  return extension_meta;
}
