#ifndef SETTINGS_H
#define SETTINGS_H

#include "core-kit/basis.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/explanationlabel.h"
#include "widgets/lineedit.h"
#include "widgets/layouts/linears.h"
#include <QCheckBox>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTranslator>
#include <QWidget>

/*! @class Settings
 *  @brief Manages the program settings.
 *  @sa Basis, ModalHandler  */
class Settings : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Settings)
public:
  // Functions described in `settings.cpp`:
  Settings(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  VLineLt *layout = nullptr;
  QCheckBox delay, keepHistory, greetings;
  QWidget delayBox;
  QSpinBox minDelay, maxDelay;
  LineEdit greetingsMsg;
  Button save_and_close;

  // Functions described in `settings.cpp`:
  void loadStates();
  void saveAndClose();
  void delayChecked();
  void minDelayValueChanged(int value);
  void maxDelayValueChanged(int value);
  void greetingsChecked();
};

#endif
