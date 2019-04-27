#include "nlp-module.h"

AMessage *NLPmodule::get(const QString &ue, AMessage::A a, QWidget *p) {
  // Prepares user expression...
  auto *msg = new AMessage(p);
  msg->setAuthor(a);
  QString pue = this->preparePlugins(ue, msg);
  if (msg->returnMessageType() == AMessage::Widget) return msg;
  msg->setMessageType(AMessage::HTML, this->think(pue /*, msg*/));
  return msg;
}

QString NLPmodule::think(const QString &ue /*, AkiwakeMessage *msg*/) {
  // For each container in AS selection...
  QList<linksMicroMap> cms;
  auto *sq = new sqlite();
  SettingsStore st;
  for (containerProperties cProp : st.read()) {
    // Gets the activators found in the custom expression and the reagent links
    QList<QPair<QString, QString>> al = sq->scan(cProp.p, cProp.c, ue);
    QList<containerRow> cm;
    bool _aProp = sq->hasAdditionalProperties(cProp.p, cProp.c);
    // For each mind connection...
    for (QPair<QString, QString> mc : al) {
      QStringList links = mc.second.split(",");
      // ...we get a lot of single connections...
      for (QString link : links) {
        containerRow el;
        el.a = mc.first;
        el.rAdd = link.toInt();
        if (_aProp) el.rProps = sq->scanAP(cProp.p, cProp.c, el.rAdd);
        cm.append(el);
      }
    }
    // We have the container row's list now...
    linksMicroMap cmBox = handlePlugins(cm, _aProp /*, msg*/);
    cmBox.pr = cProp;
    cms.append(cmBox);
  }
  // And we have the all needed materials. Let's compose it...
  globalExpressionNetworkMap genm = this->microMapCombinator(sq, cms);
  delete sq;
  return this->selectReagents(ue, genm);
}

QString NLPmodule::preparePlugins(QString ue, AMessage *msg) {
  // Handles expression by Python scripts and built-in methods.
  // Here will be the expression processing code in scripts...
  standardTemplates stp;
  ue = stp.dialogues(ue, msg);
  // < ... >
  return this->simplifier(ue);
}

QString NLPmodule::simplifier(const QString &ex) {
  handlers hd;
  return hd.purifyString(ex);
}

linksMicroMap NLPmodule::handlePlugins(const QList<containerRow> &cm,
                                       bool _aProp /*, AkiwakeMessage *msg*/) {
  // Handles all answer expressions by Python scripts and built-in methods.
  linksMicroMap cmBox;
  QMap<QString, QList<int>> cls;
  for (containerRow el : cm) {
    // If there isn't current activator in compressed lines...
    el.a = this->simplifier(el.a);
    if (!cls.contains(el.a)) {
      QList<int> addresses;
      cls.insert(el.a, addresses);
    }
    // If there isn't current address in list of addresses of the current
    // activator in compressed lines...
    if (!cls.value(el.a).contains(el.rAdd)) {
      QList<int> addresses = cls.value(el.a);
      addresses.append(el.rAdd);
      cls.insert(el.a, addresses);
    }
  }
  if (_aProp) {
    // < ... >
  }
  cmBox.el = cls;
  return cmBox;
}

globalExpressionNetworkMap NLPmodule::microMapCombinator(
    sqlite *sq, const QList<linksMicroMap> &sRes) {
  globalExpressionNetworkMap genm;
  for (linksMicroMap mm : sRes) {
    for (QList<int> ls : mm.el) {
      QString a = mm.el.key(ls);
      QStringList rs;
      if (genm.ar.contains(a)) rs = genm.ar.value(a);
      for (int l : ls) {
        QString r = sq->scan(mm.pr.p, mm.pr.c, l).first;
        if ((!rs.contains(r)) && (r != "")) rs.append(r);
      }
      genm.ar.insert(a, rs);
    }
  }
  return genm;
}

QStringList NLPmodule::sorting(const QString &ue, QStringList ks) {
  if (ks.length() <= 1) return ks;
  QString pn = ks.takeAt(int(ks.length() / 2));
  QStringList esl;
  QStringList lsl;
  for (QString k : ks) {
    if (ue.indexOf(k) > ue.indexOf(pn))
      lsl.append(k);
    else
      esl.append(k);
  }
  QStringList sd;
  esl = this->sorting(ue, esl);
  for (QString key : esl) sd.append(key);
  sd.append(pn);
  lsl = this->sorting(ue, lsl);
  for (QString key : lsl) sd.append(key);
  return sd;
}

QString NLPmodule::selectReagents(QString ue,
                                  const globalExpressionNetworkMap &genm) {
  if (genm.ar.keys().length() == 0) return "";
  QStringList ks = this->sorting(ue, genm.ar.keys());
  QString tRes;
  for (QString k : ks) {
    if (!ue.contains(k)) continue;
    ue.remove(k);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    tRes += genm.ar.value(k).at(rand.bounded(genm.ar.value(k).length())) + " ";
  }
  return tRes;
}
