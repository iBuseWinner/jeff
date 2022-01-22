#ifndef NLPOPTIONS_H
#define NLPOPTIONS_H

#include <QMap>
#include <QString>

/*!
 * @enum OptionType
 * @brief 
 */
enum OptionType { ExpressionWeight, AutoDelete, Custom };

/*!
 * @typedef Options
 * @brief QMap (option_name, option_value) (uses in @a Core and other modules)
 */
typedef QMap<QString, QString> Options;

#endif
