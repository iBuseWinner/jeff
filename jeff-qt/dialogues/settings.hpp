#ifndef SETTINGS_H
#define SETTINGS_H

#include "core-kit/basis.hpp"
#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "widgets/button.hpp"
#include "widgets/explanationlabel.hpp"
#include "widgets/lineedit.hpp"
#include "widgets/layouts/linears.hpp"
#include <QCheckBox>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTranslator>
#include <QWidget>

/*! @class Settings
 *  @brief Manages the program settings.
 *  @sa Basis, ModalHandler  */
class Settings : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(Settings)
public:
  // Functions described in `settings.cpp`:
  Settings(Basis *_basis, QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);

private:
  // Objects:
  Basis *basis = nullptr;
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
