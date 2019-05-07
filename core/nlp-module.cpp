#include "nlp-module.h"

NLPmodule::NLPmodule(settings *_st, sqlite *_sq, QObject *p) : QObject(p) {
  st = _st;
  sq = _sq;
}

void NLPmodule::search(QString ue) {
  ue = simplifier(ue);
  QList<linkMap> lms;
  for (auto cProp : st->read()) {
    QList<containerRow> crs;
    QMap<QString, QString> els = sq->scan(cProp, ue);
    bool hasAddProps = sq->hasAdditionalProperties(cProp);
    for (auto mc : els.keys()) {
      QStringList links = els.value(mc).split(',');
      for (auto link : links) {
        containerRow el;
        el.a = mc;
        el.ra = link.toInt();
        if (hasAddProps) el.rProps = sq->scanAP(cProp, el.ra);
        crs.append(el);
      }
    }
    linkMap lm = toLinkMap(crs, hasAddProps);
    lm.cProp = cProp;
    lms.append(lm);
  }
  select(ue, toGlobalMap(lms));
}

linkMap NLPmodule::toLinkMap(const QList<containerRow> &cm, bool _aProp) {
  linkMap lm;
  QMap<QString, QList<int>> cls;
  for (containerRow el : cm) {
    el.a = simplifier(el.a);
    if (!cls.contains(el.a)) {
      QList<int> addresses;
      cls.insert(el.a, addresses);
    }
    if (!cls.value(el.a).contains(el.ra)) {
      QList<int> addresses = cls.value(el.a);
      addresses.append(el.ra);
      cls.insert(el.a, addresses);
    }
  }
  if (_aProp) {
    // < ... >
  }
  lm.rl = cls;
  return lm;
}

globalMap NLPmodule::toGlobalMap(const QList<linkMap> &sRes) {
  globalMap genm;
  for (auto mm : sRes) {
    for (auto ls : mm.rl) {
      QString a = mm.rl.key(ls);
      QStringList rs;
      if (genm.ar.contains(a)) rs = genm.ar.value(a);
      for (int l : ls) {
        QString r = sq->scan(mm.cProp, l).first;
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
  esl = sorting(ue, esl);
  for (QString key : esl) sd.append(key);
  sd.append(pn);
  lsl = sorting(ue, lsl);
  for (QString key : lsl) sd.append(key);
  return sd;
}

void NLPmodule::select(QString ue, const globalMap &genm) {
  if (genm.ar.keys().length() == 0) return;
  QStringList ks = sorting(ue, genm.ar.keys());
  QString s;
  for (QString k : ks) {
    if (!ue.contains(k)) continue;
    ue.remove(k);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    s += genm.ar.value(k).at(rand.bounded(genm.ar.value(k).length())) + " ";
  }
  emit ready(s.trimmed());
}

QString NLPmodule::simplifier(const QString &ex) {
  return handlers::purify(ex);
}
