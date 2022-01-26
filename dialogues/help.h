#ifndef HELP_H
#define HELP_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

/*!
 * @class Help
 * @brief Tells users how to use this program.
 */
class Help : public QWidget {
  Q_DISABLE_COPY(Help)
public:
  // Functions described in `help.cpp`:
  Help(QWidget *parent = nullptr);
};

#endif
