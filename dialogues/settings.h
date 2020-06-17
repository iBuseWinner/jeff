#ifndef SETTINGS_H
#define SETTINGS_H

#include "core/basis.h"
#include "dialogues/modal-handler.h"
#include "widgets/a_button.h"
#include "widgets/a_explanationlabel.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTranslator>
#include <QWidget>

/*!
 * Class: Settings.
 * Manages the program settings.
 */
class Settings : public QWidget {
  Q_OBJECT
public:
  // Functions:
  explicit Settings(Basis *_basis, QWidget *parent = nullptr,
                    ModalHandler *m_handler = nullptr);

private:
  Q_DISABLE_COPY(Settings)

  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  inline static const QString objn = "settings";
  QCheckBox *delay = nullptr;
  QWidget *box1 = nullptr;
  QSpinBox *minDelay = nullptr;
  QSpinBox *maxDelay = nullptr;
  QCheckBox *keepHistory = nullptr;
  AButton *save_and_close = nullptr;

  // Functions:
  void connector();
  void loadStates();
  void saveAndClose();
  void delayChecked() { box1->setEnabled(delay->isChecked()); }
  void minDelayValueChanged(int value);
  void maxDelayValueChanged(int value);
};

#endif // SETTINGS_H
