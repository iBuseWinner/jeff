#include "settings.h"

settings::settings(QObject *p) : QObject(p) {
  if (!s->isWritable()) emit roWarning();
}

QVariant settings::read(const QString &k) { return s->value(k); }

QList<container> settings::read() {
  QString sPath = settingsPath();
  auto *sf = new QFile(sPath + QDir::separator() + sfn, this);
  QJsonArray cs = r(sf);
  QList<container> cProps;
  sqlite sq;
  for (auto i : cs) {
    container cp = toC(i.toObject());
    cp = sq.optionsLoader(cp);
    cProps.append(cp);
  }
  return cProps;
}

QList<message> settings::read(QFile *sf) {
  QJsonArray ms = r(sf);
  QList<message> mh;
  for (auto i : ms) {
    message m = toM(i.toObject());
    mh.append(m);
  }
  return mh;
}

void settings::write(const QString &k, const QVariant &d) { s->setValue(k, d); }

void settings::write(QList<container> cp) {
  QString sPath = settingsPath();
  QJsonArray cs;
  sqlite sq;
  for (const auto &i : cp) {
    cs.append(toJ(i));
    sq.optionsWriter(i);
  }
  QJsonDocument dbl(cs);
  QFile sf(sPath + QDir::separator() + sfn);
  if (!sf.open(QIODevice::WriteOnly | QIODevice::Text)) return;
  QTextStream st(&sf);
  st << dbl.toJson(QJsonDocument::Indented);
  sf.close();
}

void settings::write(QList<message> mh, QFile *sf) {
  QJsonArray ms;
  for (const auto &i : mh) ms.append(toJ(i));
  QJsonDocument ml(ms);
  if (!sf->open(QIODevice::WriteOnly | QIODevice::Text)) return;
  QTextStream st(sf);
  st << ml.toJson(QJsonDocument::Indented);
  sf->close();
}

QString settings::settingsPath() {
  QFileInfo fi(s->fileName());
  QString p = fi.absolutePath();
  return p;
}

QJsonArray settings::r(QFile *sf) {
  if (!sf->open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonArray();
  QTextStream ts(sf);
  auto *err = new QJsonParseError();
  QJsonDocument dbl = QJsonDocument::fromJson(ts.readAll().toUtf8(), err);
  if (err->error != QJsonParseError::NoError) {
    emit jsonError(err->errorString());
    delete err;
    return QJsonArray();
  }
  delete err;
  sf->close();
  return dbl.array();
}

QJsonObject settings::toJ(const container &cProp) {
  QJsonObject converted{{"container", cProp.c},
                        {"disabled", cProp.d},
                        {"path", cProp.p},
                        {"title", cProp.t}};
  return converted;
}

QJsonObject settings::toJ(const message &daemon) {
  QJsonObject converted{{"content", daemon.cn},
                        {"datetime", daemon.dt},
                        {"author", int(daemon.aType)},
                        {"contentType", int(daemon.cType)}};
  return converted;
}

container settings::toC(const QJsonObject &obj) {
  container cProp;
  cProp.c = obj.value("container").toString();
  cProp.d = obj.value("disabled").toBool();
  cProp.p = obj.value("path").toString();
  cProp.t = obj.value("title").toString();
  return cProp;
}

message settings::toM(const QJsonObject &obj) {
  message daemon;
  daemon.cn = obj.value("content").toString();
  daemon.dt = obj.value("datetime").toString();
  daemon.aType = A(obj.value("author").toInt());
  daemon.cType = C(obj.value("contentType").toInt());
  return daemon;
}
