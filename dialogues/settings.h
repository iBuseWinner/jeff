#ifndef SETTINGS_H
#define SETTINGS_H

#include "core/basis.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/explanationlabel.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTranslator>
#include <QWidget>

/*!
 * @class Settings
 * @brief Manages the program settings.
 * @sa Basis, ModalHandler
 */
class Settings : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Settings)
public:
  // Functions described in `settings.cpp`:
  explicit Settings(Basis *_basis, QWidget *parent = nullptr,
                    ModalHandler *m_handler = nullptr);

private:
  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  QCheckBox *delay = nullptr;
  QWidget *box1 = nullptr;
  QSpinBox *minDelay = nullptr;
  QSpinBox *maxDelay = nullptr;
  QCheckBox *keepHistory = nullptr;
  Button *save_and_close = nullptr;

  // Constants:
  inline static const QString object_name = "settings";

  // Functions:
  /*!
   * @fn Settings::delayChecked
   * @brief Depending on whether the delay is enabled, turns on/off the widget
   * with a choice of delay boundaries.
   */
  void delayChecked() { box1->setEnabled(delay->isChecked()); }

  // Functions described in `settings.cpp`:
  void connector();
  void loadStates();
  void saveAndClose();
  void minDelayValueChanged(int value);
  void maxDelayValueChanged(int value);
};

#endif
