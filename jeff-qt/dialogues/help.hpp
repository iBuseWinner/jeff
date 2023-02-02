#ifndef HELP_H
#define HELP_H

#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "widgets/button.hpp"
#include "widgets/scrollarea.hpp"
#include "widgets/layouts/grid.hpp"
#include "widgets/layouts/linears.hpp"
#include <QLabel>
#include <QSpacerItem>
#include <QWidget>

/*! @enum HelpTipListDirection
 *  @brief TBD  */
enum HelpTipListDirection { NextTip, PreviousTip };

/*! @class Help
 *  @brief Tells users how to use this program.  */
class Help : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(Help)
public:
  // Functions described in `help.cpp`:
  Help(QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);
  void set_tip(HelpTipListDirection direction = NextTip);
  
private:
  // Objects:
  QLabel *title = nullptr, *text = nullptr;
  int curr_tip = 0;
  void _set_tip(int tip_number = 0);
};

#endif
