#include "nlp-module.h"
#ifdef NLPMODULE_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

/*!
 * Argument: QString {user_expression} [user input expression].
 * Runs regular expression searches in databases in several stages:
 *   1) purifies expression {user_expression};
 *   2) scans every source {source} for a regular expressions;
 *   3) selects the results and sends them to the core.
 */
void NLPmodule::search(QString user_expression) {
#ifdef NLPMODULE_SEARCH_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  // 1)
  user_expression = basis->sql->purify(user_expression);
  // 2)
  QList<LinkMap> link_map_list;
  QList<Source> sources = basis->getSources();
  for (const Source &source : sources) {
    QList<SourceRow> source_row_list;
    // First receives a map {activator_links} of activators in
    // {user_expression}. The first argument is the activator, the second is the
    // references to the reagents {links}.
    QMap<QString, QString> activator_links =
        basis->sql->scanSource(source, user_expression);
    bool has_additional_properties =
        basis->sql->hasAdditionalProperties(source);
    for (const QString &activator : activator_links.keys()) {
      QStringList links = activator_links[activator].split(',');
      for (const QString &link : qAsConst(links)) {
        SourceRow source_row;
        source_row.activator = activator;
        source_row.reagent_address = link.toInt();
        // Among other things, SourceRow retains additional properties. For
        // each expression, they are different.
        if (has_additional_properties)
          source_row.reagent_properties = basis->sql->scanAdditionalProperties(
              source, source_row.reagent_address);
        source_row_list.append(source_row);
      }
    }
    // The list of SourceRow {source_row_list} turns into a LinkMap.
    // As a result of executing toLinkMap, additional properties are processed,
    // individual response options are discarded, added or changed.
    // [!] It is important to process additional properties now, in order not to
    // waste machine time and memory on this later.
    LinkMap link_map = toLinkMap(source_row_list, has_additional_properties);
    // To convert links to reagents, source information is recorded in
    // LinkMap {link_map}. Thus, one LinkMap is formed per source.
    link_map.source = source;
    link_map_list.append(link_map);
  }
  // 3)
  // The LinkMap list {link_map_list} turns into a list of activator-reagent
  // pairs. Thus, information from databases is finally transformed into a
  // structured data set.
  select(user_expression, toGlobalMap(link_map_list));
#ifdef NLPMODULE_SEARCH_DEBUG
  qDebug() << "NLPmodule::search:" << timer.nsecsElapsed();
#endif
}

/*!
 * Arguments: QList of SourceRow {source_row_list}
 *                [list of rows of the table to be converted to a LinkMap],
 *            bool {has_additional_properties}
 *                [reports whether the table has additional properties].
 * Turns the SourceRow list into a LinkMap, handles additional properties.
 * Returns: LinkMap.
 */
LinkMap NLPmodule::toLinkMap(QList<SourceRow> source_row_list,
                             bool has_additional_properties) {
  // Further, additional properties will be discarded, so that they are
  // processed here.
  if (has_additional_properties) {
    // < ... >
  }
  LinkMap link_map;
  for (SourceRow source_row : source_row_list) {
    source_row.activator = basis->sql->purify(source_row.activator);
    // Possible duplicates are discarded here, the activator reagents are
    // combined.
    if (link_map.activator_links[source_row.activator].contains(
            source_row.reagent_address))
      continue;
    QList<int> addresses;
    if (not link_map.activator_links[source_row.activator].isEmpty())
      addresses = link_map.activator_links.take(source_row.activator);
    addresses.append(source_row.reagent_address);
    link_map.activator_links.insert(source_row.activator, addresses);
  }
  return link_map;
}

/*!
 * Argument: QList of LinkMap {link_map_list}
 *               [list of all LinkMaps to form a GlobalMap].
 * Gets reagents on the links, forms GlobalMap {global_map}.
 * Returns: map of activators and their reagents.
 */
GlobalMap NLPmodule::toGlobalMap(const QList<LinkMap> &link_map_list) {
  GlobalMap global_map;
  for (const LinkMap &link_map : link_map_list) {
    for (const QList<int> &links : qAsConst(link_map.activator_links)) {
      QString ac = link_map.activator_links.key(links);
      // Possible duplicates are discarded here, the activator reagents are
      // combined.
      QStringList reagents;
      if (global_map.activator_reagents.contains(ac))
        reagents = global_map.activator_reagents[ac];
      for (int link : qAsConst(links)) {
        QString rg = basis->sql->getExpression(link_map.source, link).first;
        if ((not reagents.contains(rg)) && (rg != ""))
          reagents.append(rg);
      }
      global_map.activator_reagents.insert(ac, reagents);
    }
  }
  return global_map;
}

/*!
 * Arguments: QString {user_expression} [user input expression],
 *            QStringList {activators} [activators to be sorted].
 * Sorts activators depending on the place of entry. This is necessary in order
 * for the NLP-module to give answers in which the reagents were in the order
 * given in {user_expression}, for example:
 *    1) ue: "Hello! How are you?"
 *       ae: "Hi! Fine."
 *    2) ue: "How are you? Hello!"
 *       ae: "Fine. Hi!"
 * Algorithm: insertion sort.
 * Returns: sorted list of activators.
 */
QStringList NLPmodule::sorting(const QString &user_expression,
                               QStringList activators) {
  if (activators.length() <= 1)
    return activators;
  QString section = activators.takeAt(int(activators.length() / 2));
  QStringList earlierActivators;
  QStringList laterActivators;
  for (const QString &activator : qAsConst(activators)) {
    if (user_expression.indexOf(activator) > user_expression.indexOf(section))
      laterActivators.append(activator);
    else
      earlierActivators.append(activator);
  }
  QStringList sorted;
  earlierActivators = sorting(user_expression, earlierActivators);
  for (const QString &activator : qAsConst(earlierActivators))
    sorted.append(activator);
  sorted.append(section);
  laterActivators = sorting(user_expression, laterActivators);
  for (const QString &activator : qAsConst(laterActivators))
    sorted.append(activator);
  return sorted;
}

/*!
 * Arguments: QString {user_expression} [user input expression],
 *            globalMap {global_map} [map of activators and reagents].
 * Of the proposed equivalent reagents randomly selects one by one, forming a
 * response expression.
 */
void NLPmodule::select(QString user_expression, const GlobalMap &global_map) {
  QStringList activators =
      sorting(user_expression, global_map.activator_reagents.keys());
  QString response_expression;
  for (const QString &activator : qAsConst(activators)) {
    if (not user_expression.contains(activator))
      continue;
    user_expression.remove(activator);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    if (global_map.activator_reagents[activator].isEmpty())
      continue;
    response_expression +=
        global_map.activator_reagents[activator][rand.bounded(
            global_map.activator_reagents[activator].length())] +
        " ";
  }
  emit ready(response_expression.trimmed());
}
