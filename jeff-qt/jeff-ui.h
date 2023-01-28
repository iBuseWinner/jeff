#ifndef JEFF_H
#define JEFF_H

#include "core-kit/core.h"
#include "widgets/board.h"
#include "widgets/button.h"
#include "widgets/display.h"
#include "widgets/line.h"
#include "widgets/menubar.h"
#include "widgets/styling.h"
#include "widgets/textedit.h"
#include "widgets/layouts/linears.h"
#include <QApplication>
#include <QCloseEvent>
#include <QLayout>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QPair>
#include <QResizeEvent>
#include <QWidget>

/*! @class Jeff
 *  @brief Application window class.  */
class Jeff : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(Jeff)
public:
  // Functions:
  /*! @brief Saves window settings. */
  ~Jeff() override { save_window_settings(); }

  // Functions described in `jeff-ui.cpp`:
  Jeff();

signals:
  /*! @brief Indicates that the window is complete and ready for user interaction. */
  void ready_state();
  /*! @brief Sends a request to Core. */
  QString send(QString user_expression);

protected:
  // Functions described in `jeff-ui.cpp`:
  void keyPressEvent(QKeyEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

private:
  // Objects:
  Core *core = new Core(this);
  Basis *basis = core->basis;
  HProcessor *history_processor = core->hp;
  Line *line = new Line(this);
  Display *display = new Display(history_processor, 25, this);
  MenuBar *menubar = new MenuBar(core, line, this);

  // Constants:
  static const int minimalWidth = 800;
  static const int minimalHeight = 370;
  static const int defaultWidth = 800;
  static const int defaultHeight = 496;

  // Functions described in `jeff-ui.cpp`:
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
