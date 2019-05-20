#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCheckBox>
#include <QGridLayout>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTranslator>
#include <QWidget>
#include "core/core-methods.h"
#include "widgets/a_button.h"
#include "widgets/settings/a_explanationlabel.h"

class Settings : public QWidget {
  Q_OBJECT
 public:
  // Functions:
  explicit Settings(CoreMethods *_Meths, QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(Settings)

  // Objects:
  CoreMethods *Meths = nullptr;
  const QString objn = "settings";
  QCheckBox *monologue = nullptr;
  QCheckBox *delay = nullptr;
  QWidget *box1 = nullptr;
  QSpinBox *minDelay = nullptr;
  QSpinBox *maxDelay = nullptr;
  QCheckBox *keepHistory = nullptr;
  AButton *snclBtn = nullptr;

  // Functions:
  void connector();
  void loadStates();
  void saveAndClose();
  void delayChecked() { box1->setEnabled(delay->isChecked()); }
  void minDelayValueChanged(int value) { maxDelay->setMinimum(value); }
  void maxDelayValueChanged(int value) { minDelay->setMaximum(value); }
};

#endif  // SETTINGS_H
