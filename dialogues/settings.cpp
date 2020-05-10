#include "settings.h"

/*
 * All short named objects and their explanations:
 * {Meths} <- core methods
 * {lt} <- layout
 * {snclBtn} <- save and close
 */

/*!
 * Arguments: CoreMethods {_Meths} [reference to CoreMethods instance],
 *            QWidget {*parent}.
 * Constructs and prepares Settings.
 */
Settings::Settings(CoreMethods *_Meths, QWidget *parent) : QWidget(parent) {
  Meths = _Meths;
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(objn);
  auto *lt = new QVBoxLayout();
  lt->setSpacing(0);
  lt->setMargin(0);
  monologue =
      new QCheckBox(QTranslator::tr("Enable monologue mode support"), this);
  delay = new QCheckBox(QTranslator::tr("Enable delay"), this);
  auto *minDelayText = new QLabel(QTranslator::tr("From"), this);
  minDelayText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  minDelay = new QSpinBox(this);
  minDelay->setMaximum(5000);
  minDelay->setSingleStep(10);
  auto *maxDelayText = new QLabel(QTranslator::tr("to"), this);
  maxDelayText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  maxDelay = new QSpinBox(this);
  maxDelay->setMaximum(5000);
  maxDelay->setSingleStep(10);
  auto *msText = new QLabel(QTranslator::tr("ms"), this);
  msText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  box1 = new QWidget(this);
  auto *box1Lt = new QHBoxLayout();
  box1Lt->addWidget(minDelayText);
  box1Lt->addWidget(minDelay);
  box1Lt->addWidget(maxDelayText);
  box1Lt->addWidget(maxDelay);
  box1Lt->addWidget(msText);
  box1->setLayout(box1Lt);
  keepHistory = new QCheckBox(QTranslator::tr("Keep history"), this);
  keepHistory->setEnabled(false);
  auto *monologueExplanation = new AExplanationLabel(
      QTranslator::tr(
          "ASW will be able to talk to himself (restart is required)."),
      this);
  auto *delayExplanation = new AExplanationLabel(
      QTranslator::tr("Enables ASW response delay."), this);
  auto *keepHistoryExplanation = new AExplanationLabel(
      QTranslator::tr(
          "Enables keeping the message history after exiting the application."),
      this);
  snclBtn = new AButton(QTranslator::tr("OK"), this);
  snclBtn->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  lt->addWidget(monologue);
  lt->addWidget(monologueExplanation);
  lt->addWidget(delay);
  lt->addWidget(delayExplanation);
  lt->addWidget(box1);
  lt->addWidget(keepHistory);
  lt->addWidget(keepHistoryExplanation);
  lt->addWidget(snclBtn);
  setLayout(lt);
  connector();
  loadStates();
}

/*! Establishes communications for user interaction through the dialog box. */
void Settings::connector() {
  connect(delay, &QCheckBox::toggled, this, &Settings::delayChecked);
  connect(minDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::minDelayValueChanged);
  connect(maxDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::maxDelayValueChanged);
  connect(snclBtn, &AButton::clicked, this, &Settings::saveAndClose);
}

/*! Loads settings from file. */
void Settings::loadStates() {
  monologue->setChecked(Meths->read(Meths->isMonologueModeEnabledSt).toBool());
  delay->setChecked(Meths->read(Meths->isDelayEnabledSt).toBool());
  delayChecked();
  keepHistory->setChecked(Meths->read(Meths->isKeepingEnabledSt).toBool());
  minDelay->setValue(Meths->read(Meths->minDelaySt).toInt());
  minDelayValueChanged(minDelay->value());
  maxDelay->setValue(Meths->read(Meths->maxDelaySt).toInt());
  maxDelayValueChanged(maxDelay->value());
}

/*! Saves ASW settings. */
void Settings::saveAndClose() {
  Meths->write(Meths->isMonologueModeEnabledSt, monologue->isChecked());
  Meths->write(Meths->isDelayEnabledSt, delay->isChecked());
  Meths->write(Meths->isKeepingEnabledSt, keepHistory->isChecked());
  Meths->write(Meths->minDelaySt, minDelay->value());
  Meths->write(Meths->maxDelaySt, maxDelay->value());
  close();
}

void Settings::minDelayValueChanged(int value) {
  if (minDelay->value() > maxDelay->value())
    maxDelay->setValue(value);
}

void Settings::maxDelayValueChanged(int value) {
  if (maxDelay->value() < minDelay->value())
    minDelay->setValue(value);
}
