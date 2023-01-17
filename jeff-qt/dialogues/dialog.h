#ifndef JEFF_DIALOG_WIDGET_H
#define JEFF_DIALOG_WIDGET_H

#include "dialogues/modal-handler.h"
#include "widgets/scrollfreezerwidget.h"
#include <QWidget>

/*! @class Dialog
 *  @brief Meta-widget to simplify widget creating.  */
class Dialog : public ScrollFreezerWidget {
public:
  /*! @brief The constructor. */
  Dialog(ModalHandler *mhandler, QWidget *parent = nullptr)
    : ScrollFreezerWidget(parent), _mhandler(mhandler)
  {
    _mhandler->setPrisoner(this);
  }
  /*! @brief Closes this dialog. */
  void close() { _mhandler->closePrisoner(); }
  
private:
  ModalHandler *_mhandler = nullptr; /*!< @sa ModalHandler */
};

#endif
