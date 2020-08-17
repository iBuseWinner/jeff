#ifndef ASW_H
#define ASW_H

#include "core/core.h"
#include "widgets/a_board.h"
#include "widgets/a_button.h"
#include "widgets/a_display.h"
#include "widgets/a_line.h"
#include "widgets/a_menubar.h"
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
 * @mainclass ASW
 * @brief Application window class.
 */
class ASW : public QMainWindow {
  Q_OBJECT
public:
  // Functions:
  /*!
   * @fn ASW::~ASW
   * @brief Saves window settings.
   */
  ~ASW() override { saveWindowSettings(); }

  // Functions described in 'asw.cpp':
  explicit ASW();
  void greeting();

signals:
  /*!
   * @brief Indicates that the window is complete and ready for user
   * interaction.
   */
  void readyState();

  /*!
   * @brief Sends a request to Core.
   * @sa Core
   */
  QString send(QString user_expression);

protected:
  // Functions described in 'asw.cpp':
  void keyPressEvent(QKeyEvent *event) override;

private:
  Q_DISABLE_COPY(ASW)

  // Objects:
  Core *core = new Core(this);
  Basis *basis = core->basis;
  HProcessor *historyProcessor = core->history_processor;
  ALine *line = new ALine(this);
  ADisplay *display = new ADisplay(25, this);
  AMenuBar *menubar = new AMenuBar(line, this);

  // Constants:
  static const int minimalWidth = 600;
  static const int minimalHeight = 370;
  static const int defaultWidth = 800;
  static const int defaultHeight = 496;

  // Functions described in 'asw.cpp':
  void applyingSettings();
  void saveWindowSettings();
  void connector();
  void fullScreenHandler();
  void userInputHandler();
  void exportMessageHistory();
  void importMessageHistory();
  void clear();
  // void help() { emit send("/help"); }
};

#endif // ASW_H
