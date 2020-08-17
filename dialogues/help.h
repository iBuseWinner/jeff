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
public:
  // Functions described in 'help.cpp':
  explicit Help(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(Help)
};

#endif // HELP_H
