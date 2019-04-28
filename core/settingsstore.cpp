#include "settingsstore.h"

SettingsStore::~SettingsStore() {
  // Prevents memory leaks.
  delete this->s;
}

QVariant SettingsStore::read(const QString &k) {
  // Reads the setting by key.
  return this->s->value(k);
}

QList<containerProperties> SettingsStore::read() {
  // Reads selection of AS from file.
  QString sPath = this->settingsPath();
  QFile sf(sPath + QDir::separator() + sfn);
  if (!sf.open(QIODevice::ReadOnly | QIODevice::Text))
    return QList<containerProperties>();
  QTextStream ts(&sf);
  auto *err = new QJsonParseError;
  QJsonDocument dbl = QJsonDocument::fromJson(ts.readAll().toUtf8(), err);
  if (err->error != QJsonParseError::NoError)
    return QList<containerProperties>();
  delete err;
  sf.close();
  QJsonArray cs = dbl.array();
  QList<containerProperties> cps;
  auto *sq = new sqlite();
  for (auto i : cs) {
    containerProperties cp = this->toContainerStruct(i.toObject());
    cp = sq->optionsLoader(cp);
    cps.append(cp);
  }
  delete sq;
  return cps;
}

void SettingsStore::write(const QString &k, const QVariant &d) {
  // Records the setting by key.
  this->s->setValue(k, d);
}

void SettingsStore::write(const QList<containerProperties> &cp) {
  // Writes selection of AS into file.
  QString sPath = this->settingsPath();
  QJsonArray cs;
  sqlite sq;
  for (const auto &i : cp) {
    cs.append(this->toJson(i));
    sq.optionsWriter(i);
  }
  QJsonDocument DatabaseList(cs);
  QFile sf(sPath + QDir::separator() + sfn);
  if (!sf.open(QIODevice::WriteOnly | QIODevice::Text)) return;
  QTextStream st(&sf);
  st << DatabaseList.toJson(QJsonDocument::Indented);
  sf.close();
}

QString SettingsStore::settingsPath() {
  // Returns the setting path.
  QFileInfo fi(s->fileName());
  QString p = fi.absolutePath();
  return p;
}

QJsonObject SettingsStore::toJson(const containerProperties &cp) {
  // Converts container structure into JSON object.
  QJsonObject converted{
      {"container", cp.c}, {"disabled", cp.d}, {"path", cp.p}, {"title", cp.t}};
  return converted;
}

containerProperties SettingsStore::toContainerStruct(const QJsonObject &obj) {
  // Converts JSON object into container structure.
  containerProperties cp;
  cp.c = obj.value("container").toString();
  cp.d = obj.value("disabled").toBool();
  cp.p = obj.value("path").toString();
  cp.t = obj.value("title").toString();
  return cp;
}
