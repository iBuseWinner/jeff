#include "settings.hpp"

/*! @brief The constructor. */
Settings::Settings(Basis *_basis, QWidget *parent, ModalHandler *mhandler)
  : Dialog(mhandler, parent), basis(_basis)
{
  // Delay.
  delay.setText(tr("Enable delay"));
  connect(&delay, &QCheckBox::toggled, this, &Settings::delayChecked);
  auto *delayExplanation = new ExplanationLabel(tr("Enables Jeff's response delay."), this);
  auto *minDelayText = new QLabel(tr("From"), this);
  minDelayText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  minDelay.setMaximum(5000);
  minDelay.setSingleStep(10);
  connect(&minDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::minDelayValueChanged);
  auto *maxDelayText = new QLabel(tr("to"), this);
  maxDelayText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  maxDelay.setMaximum(5000);
  maxDelay.setSingleStep(10);
  connect(&maxDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::maxDelayValueChanged);
  auto *msText = new QLabel(tr("ms"), this);
  msText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  delayBox.setLayout(HLineLt::another()
    ->addw(minDelayText)->addw(&minDelay)->addw(maxDelayText)->addw(&maxDelay)->addw(msText)
  );
  // History keeping.
  keepHistory.setText(tr("Keep history"));
  auto *keepHistoryExplanation = new ExplanationLabel(tr(
    "Enables keeping the message history after exiting the application."), this
  );
  // Greetings.
  greetings.setText(tr("Enable greetings message on start"));
  connect(&greetings, &QCheckBox::toggled, this, &Settings::greetingsChecked);
  auto *greetingsExplanation = new ExplanationLabel(
    tr("Allows you to automatically send a message to Jeff at startup so that he prepares the "
    "information you need in advance for you."), this
  );
  // Save button & layout setting.
  save_and_close.setText(tr("OK"));
  save_and_close.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&save_and_close, &Button::clicked, this, &Settings::saveAndClose);
  setLayout(VLineLt::another()
    ->addw(&delay)->addw(delayExplanation)->addw(&delayBox)->addw(&keepHistory)
    ->addw(keepHistoryExplanation)->addw(&greetings)->addw(greetingsExplanation)
    ->addw(&greetingsMsg)->addw(&save_and_close)
  );
  loadStates();
}

/*! @brief Loads settings from file. */
void Settings::loadStates() {
  delay.setChecked((*basis)[Basis::isDelayEnabledSt].toBool());
  delayChecked();
  keepHistory.setChecked((*basis)[Basis::isHistoryKeepingEnabledSt].toBool());
  minDelay.setValue((*basis)[Basis::minDelaySt].toInt());
  minDelayValueChanged(minDelay.value());
  maxDelay.setValue((*basis)[Basis::maxDelaySt].toInt());
  maxDelayValueChanged(maxDelay.value());
  greetings.setChecked((*basis)[Basis::isGreetingsEnabledSt].toBool());
  greetingsChecked();
  greetingsMsg.setText((*basis)[Basis::greetingsMsg].toString());
}

/*! @brief Saves Jeff's settings. */
void Settings::saveAndClose() {
  basis->write(Basis::isDelayEnabledSt, delay.isChecked());
  basis->write(Basis::isHistoryKeepingEnabledSt, keepHistory.isChecked());
  basis->write(Basis::minDelaySt, minDelay.value());
  basis->write(Basis::maxDelaySt, maxDelay.value());
  basis->write(Basis::isGreetingsEnabledSt, greetings.isChecked());
  basis->write(Basis::greetingsMsg, greetingsMsg.text());
  Dialog::close();
}

/*! @brief Depending on whether the delay is enabled, turns on/off the widget
 *  with a choice of delay boundaries.  */
void Settings::delayChecked() { delayBox.setEnabled(delay.isChecked()); }
/*! @brief When the startup greeting is enabled, it enables the ability to edit it. */
void Settings::greetingsChecked() { greetingsMsg.setEnabled(greetings.isChecked()); }
/*! @brief Changes maxDelay value, if minDelay value is bigger than that. */
void Settings::minDelayValueChanged(int value) { 
  if (value > maxDelay.value()) maxDelay.setValue(value); }
/*! @brief Changes minDelay value, if maxDelay value is smaller than that. */
void Settings::maxDelayValueChanged(int value) {
  if (value < minDelay.value()) minDelay.setValue(value); }
