#ifndef JEFF_H
#define JEFF_H

#include "core/core.h"
#include "widgets/board.h"
#include "widgets/button.h"
#include "widgets/display.h"
#include "widgets/line.h"
#include "widgets/menubar.h"
#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QPair>
#include <QResizeEvent>
#include <QWidget>

/*!
 * @mainclass Jeff
 * @brief Application window class.
 */
class Jeff : public QMainWindow {
  Q_OBJECT
public:
  // Functions:
  /*!
   * @fn Jeff::~Jeff
   * @brief Saves window settings.
   */
  ~Jeff() override { save_window_settings(); }

  // Functions described in 'jeff.cpp':
  Jeff();
  void greeting();

signals:
  /*!
   * @brief Indicates that the window is complete and ready for user
   * interaction.
   */
  void ready_state();

  /*!
   * @brief Sends a request to Core.
   * @sa Core
   */
  QString send(QString user_expression);

protected:
  // Functions described in 'jeff.cpp':
  void keyPressEvent(QKeyEvent *event) override;

private:
  Q_DISABLE_COPY(Jeff)

  // Objects:
  Core *core = new Core(this);
  Basis *basis = core->basis;
  HProcessor *history_processor = core->history_processor;
  Line *line = new Line(this);
  Display *display = new Display(25, this);
  MenuBar *menubar = new MenuBar(line, this);

  // Constants:
  static const int minimalWidth = 600;
  static const int minimalHeight = 370;
  static const int defaultWidth = 800;
  static const int defaultHeight = 496;

  // Functions described in 'jeff.cpp':
  void apply_settings();
  void save_window_settings();
  void connect_all();
  void full_screen_handler();
  void user_input_handler();
  void export_message_history();
  void import_message_history();
  void clear();
  void help() { emit send("/help"); }
};

#endif
