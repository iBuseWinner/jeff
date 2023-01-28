#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "widgets/button.hpp"
#include "widgets/scrollarea.hpp"
#include "widgets/layouts/grid.hpp"
#include "widgets/layouts/linears.hpp"
#include <QLabel>
#include <QSpacerItem>
#include <QTranslator>
#include <QWidget>

/*! @class FirstStart
 *  @brief Shows a quick start guide.  */
class FirstStart : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(FirstStart)
public:
  // Functions described in `firststart.cpp`:
  FirstStart(QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);
};

#endif
