#include "nlp-module.h"
#ifdef NLPMODULE_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

/*!
 * @fn NLPmodule::search
 * @brief Runs regular expression searches in databases.
 * @details Searches take place in several stages:
 *  - @a NLPmodule::search purifies @a user_expression
 *  - scans every @a source for regular expressions
 *  - selects the results and sends them to the @a Core.
 * @param user_expression user input expression
 * @sa Core
 */
void NLPmodule::search(QString user_expression) {
#ifdef NLPMODULE_SEARCH_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  /*! 1st stage. */
  user_expression = _basis->sql->purify(user_expression);
  /*! 2nd stage. */
  QList<LinkMap> link_map_list;
  QList<Source> sources = _basis->get_sources();
  for (const Source &source : sources) {
    QList<SourceRow> source_row_list;
    /*! First receives @a activator_links map of activators in @a
     * user_expression. The first argument is the activator, the second is the
     * references to the reagents @a links. */
    QMap<QString, QString> activator_links =
        _basis->sql->scan_source(source, user_expression);
    bool has_additional_properties =
        _basis->sql->has_additional_properties(source);
    for (const QString &activator : activator_links.keys()) {
      QStringList links = activator_links[activator].split(',');
      for (const QString &link : qAsConst(links)) {
        SourceRow source_row;
        source_row.activator = activator;
        source_row.reagent_address = link.toInt();
        /*! Among other things, @a SourceRow retains additional properties. For
         * each expression, they are different. */
        if (has_additional_properties)
          source_row.reagent_properties =
              _basis->sql->scan_additional_properties(
                  source, source_row.reagent_address);
        source_row_list.append(source_row);
      }
    }
    /*! The list of SourceRow @a source_row_list turns into @a LinkMap.
     * As a result of executing @a NLPmodule::to_link_map, additional properties
     * are processed, individual response options are discarded, added or
     * changed.
     * @attention It is important to process additional properties now, in order
     * not to waste machine time and memory on this later. */
    LinkMap link_map = to_link_map(source_row_list, has_additional_properties);
    /*! To convert links to reagents, source information is recorded in
    // LinkMap @a link_map. Thus, one LinkMap is formed per source. */
    link_map.source = source;
    link_map_list.append(link_map);
  }
  /*! 3rd stage.
   * The LinkMap list @a link_map_list turns into a list of activator-reagent
   * pairs. Thus, information from databases is finally transformed into a
   * structured data set. */
  select(user_expression, to_global_map(link_map_list));
#ifdef NLPMODULE_SEARCH_DEBUG
  qDebug() << "NLPmodule::search:" << timer.nsecsElapsed();
#endif
}

/*!
 * @fn NLPmodule::to_link_map
 * @brief Turns the SourceRow list into a LinkMap, handles additional properties
 * @param source_row_list list of rows of the table to be converted to a LinkMap
 * @param has_additional_properties reports whether the table has additional
 * properties
 * @returns the resulting LinkMap.
 */
LinkMap NLPmodule::to_link_map(QList<SourceRow> source_row_list,
                               bool has_additional_properties) {
  /*! Further, additional properties will be discarded, so that they are
   * processed here. */
  if (has_additional_properties) {
    // < ... >
  }
  LinkMap link_map;
  for (SourceRow source_row : source_row_list) {
    source_row.activator = _basis->sql->purify(source_row.activator);
    /*! Possible duplicates are discarded here, the activator reagents are
     * combined. */
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
 * @fn NLPmodule::to_global_map
 * @brief Gets reagents on the links and forms @a global_map.
 * @param link_map_list list of all LinkMaps to form @a GlobalMap
 * @returns map of activators and their reagents.
 */
GlobalMap NLPmodule::to_global_map(const QList<LinkMap> &link_map_list) {
  GlobalMap global_map;
  for (const LinkMap &link_map : link_map_list) {
    for (const QList<int> &links : qAsConst(link_map.activator_links)) {
      QString activator = link_map.activator_links.key(links);
      /*! Possible duplicates are discarded here, the activator reagents are
       * combined. */
      QStringList reagents;
      if (global_map.activator_reagents.contains(activator))
        reagents = global_map.activator_reagents[activator];
      for (int link : qAsConst(links)) {
        QString reagent =
            _basis->sql->get_expression(link_map.source, link).first;
        if ((not reagents.contains(reagent)) and (not reagent.isEmpty()))
          reagents.append(reagent);
      }
      global_map.activator_reagents.insert(activator, reagents);
    }
  }
  return global_map;
}

/*!
 * @fn NLPmodule::sorting
 * @brief Sorts activators depending on the place of entry and length of whole
 * substring.
 * @details This is necessary in order for the NLPmodule to give answers in
 * which the reagents were in the order given in @a user_expression, for
 * example:
 *  - ue: "Hello! How are you?"
 *    ae: "Hi! Fine."
 *  - ue: "How are you? Hello!"
 *    ae: "Fine. Hi!"
 * Also this is necessary because if we have, for example, two expressions:
 *  `index 1 : expression "What is"       : links ...`
 *  `index 2 : expression "What is love?" : links ...`,
 * then we want to use 2nd expression always when we have "What is love,
 * darling?" or something else.
 * @param user_expression user input expression
 * @param activators activators to be sorted
 * @returns sorted list of activators.
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
    else if (user_expression.indexOf(activator) < user_expression.indexOf(section))
      earlierActivators.append(activator);
    else if (activator.length() > section.length())
      earlierActivators.append(activator);
    else
      laterActivators.append(activator);
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
 * @fn NLPmodule::select
 * @brief Of the proposed equivalent reagents randomly selects one by one,
 * forming a response expression.
 * @param user_expression user input expression
 * @param global_map map of activators and reagents
 */
void NLPmodule::select(QString user_expression, const GlobalMap &global_map) {
  QStringList activators =
      sorting(user_expression, global_map.activator_reagents.keys());
#ifdef NLPMODULE_SELECT_DEBUG
  qDebug() << activators;
#endif
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
