#include "nlp-module.h"

/*
 * All short named variables and their explanations:
 * {Meths} <- core methods
 * {ac} <- single activator
 * {acs} <- activators
 * {rg} <- single reagent
 * {rgs} <- reagents
 * {ue} <- user expression
 * {re} <- answer expression
 * {cProp} <- container properties
 * {lm} <- linkMap
 * {lms} <- list of linkMap
 * {el} <- single containerRow [expression and links]
 * {crs} <- list of containerRow
 * {els} <- expressions and links
 * {ap} <- additional properties
 * {exls} <- single expression and links
 * {exl} <- expression-link pair
 * {gm} <- globalMap
 */

/*!
 * Arguments: settings {*_settings} [reference to Settings instance],
 *            QObject {*parent}.
 * Adds SQLite database, sets references to private objects.
 */
NLPmodule::NLPmodule(CoreMethods *_Meths, QObject *parent) : QObject(parent) {
  QSqlDatabase::addDatabase("QSQLITE");
  Meths = _Meths;
}

/*!
 * Argument: QString {ue} [user input expression].
 * Runs regular expression searches in databases in several stages:
 *   1) purifies expression {ue};
 *   2) scans every container {cProp} for a regular expressions;
 *   3) selects the results and sends them to the core.
 */
void NLPmodule::search(QString userExpression) {
  // 1)
  userExpression = Meths->SQL->purify(userExpression);
  // 2)
  QList<linkMap> lms;
  for (auto cProp : Meths->readContainerList()) {
    QList<containerRow> crs;
    // First receives a map {els} of activators in {ue}.
    // The first argument is the activator, the second is the references to the
    // reagents.
    QMap<QString, QString> els = Meths->SQL->scanContainer(cProp, userExpression);
    bool ap = Meths->SQL->hasAdditionalProperties(cProp);
    for (auto ac : els.keys()) {
      QStringList links = els.value(ac).split(',');
      for (auto link : links) {
        containerRow el;
        el.ac = ac;
        el.ra = link.toInt();
        // Among other things, containerRow retains additional properties. For
        // each expression, they are different.
        if (ap) el.rProps = Meths->SQL->scanAdditionalProperties(cProp, el.ra);
        crs.append(el);
      }
    }
    // The list of containerRow {crs} turns into a linkMap.
    // As a result of executing toLinkMap, additional properties are processed,
    // individual response options are discarded, added or changed.
    // [!] It is important to process additional properties now, in order not to
    // waste machine time and memory on this later.
    linkMap lm = toLinkMap(crs, ap);
    // To convert links to reagents, container information is recorded in
    // linkMap {lm}. Thus, one linkMap is formed per container.
    lm.cProp = cProp;
    lms.append(lm);
  }
  // 3)
  // The linkMap {lms} turns into a list of activator-reagent pairs.
  // Thus, information from databases is finally transformed into a structured
  // data set.
  select(userExpression, toGlobalMap(lms));
}

/*!
 * Arguments: QList of containerRow {cm} [list of rows of the table to be
 * converted to a linkMap],
 *            bool {_aProp} [reports whether the table has additional
 * properties].
 * Turns the containerRow list into a linkMap, handles additional properties.
 * Returns: linkMap.
 */
linkMap NLPmodule::toLinkMap(QList<containerRow> crs, bool _aProp) {
  // Further, additional properties will be discarded, so that they are
  // processed here.
  if (_aProp) {
    // < ... >
  }
  linkMap lm;
  for (containerRow exl : crs) {
    exl.ac = Meths->SQL->purify(exl.ac);
    // Possible duplicates are discarded here, the activator reagents are
    // combined.
    if (lm.al.value(exl.ac).contains(exl.ra)) continue;
    QList<int> addresses;
    if (!lm.al.value(exl.ac).isEmpty()) addresses = lm.al.take(exl.ac);
    addresses.append(exl.ra);
    lm.al.insert(exl.ac, addresses);
  }
  return lm;
}

/*!
 * Argument: QList of linkMap {lms} [list of all linkMap to form a globalMap].
 * Gets reagents on the links, forms globalMap {gm}.
 * Returns: map of activators and their reagents.
 */
globalMap NLPmodule::toGlobalMap(const QList<linkMap> &lms) {
  globalMap gm;
  for (auto lm : lms) {
    for (auto links : lm.al) {
      QString ac = lm.al.key(links);
      // Possible duplicates are discarded here, the activator reagents are
      // combined.
      QStringList rgs;
      if (gm.ars.contains(ac)) rgs = gm.ars.value(ac);
      for (int link : links) {
        QString rg = Meths->SQL->getExpression(lm.cProp, link).first;
        if ((!rgs.contains(rg)) && (rg != "")) rgs.append(rg);
      }
      gm.ars.insert(ac, rgs);
    }
  }
  return gm;
}

/*!
 * Arguments: QString {ue} [user input expression],
 *            QStringList {as} [activators to be sorted].
 * Sorts activators depending on the place of entry. This is necessary in order
 * for the NLP-module to give answers in which the reagents were in the order
 * given in {ue}, for example:
 *    1) ue: "Hello! How are you?"
 *       ae: "Hi! Fine."
 *    2) ue: "How are you? Hello!"
 *       ae: "Fine. Hi!"
 * Algorithm: insertion sort.
 * Returns: sorted list of activators.
 */
QStringList NLPmodule::sorting(const QString &ue, QStringList as) {
  if (as.length() <= 1) return as;
  QString section = as.takeAt(int(as.length() / 2));
  QStringList earlierActivators;
  QStringList laterActivators;
  for (QString a : as) {
    if (ue.indexOf(a) > ue.indexOf(section))
      laterActivators.append(a);
    else
      earlierActivators.append(a);
  }
  QStringList sorted;
  earlierActivators = sorting(ue, earlierActivators);
  for (QString a : earlierActivators) sorted.append(a);
  sorted.append(section);
  laterActivators = sorting(ue, laterActivators);
  for (QString a : laterActivators) sorted.append(a);
  return sorted;
}

/*!
 * Arguments: QString {ue} [user input expression],
 *            globalMap {gm} [map of activators and reagents].
 * Of the proposed equivalent reagents randomly selects one by one, forming a
 * response expression.
 */
void NLPmodule::select(QString ue, const globalMap &gm) {
  QStringList as = sorting(ue, gm.ars.keys());
  QString re;
  for (QString a : as) {
    if (!ue.contains(a)) continue;
    ue.remove(a);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    re += gm.ars.value(a).at(rand.bounded(gm.ars.value(a).length())) + " ";
  }
  emit ready(re.trimmed());
}
