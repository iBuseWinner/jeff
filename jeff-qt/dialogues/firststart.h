#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "dialogues/dialog.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/scrollarea.h"
#include "widgets/layouts/grid.h"
#include "widgets/layouts/linears.h"
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
