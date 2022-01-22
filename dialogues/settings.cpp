#include "settings.h"

/*!
 * @fn Settings::Settings
 * @brief The constructor.
 * @param[in,out] _basis reference to the Basis instance
 * @param[in,out] parent QObject parent
 * @param[in,out] m_handler reference to the ModalHandler instance
 */
Settings::Settings(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : QWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(object_name);
  auto *vert_box_lt = new QVBoxLayout();
  vert_box_lt->setSpacing(0);
  vert_box_lt->setMargin(0);
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
  auto *delayExplanation = new ExplanationLabel(QTranslator::tr("Enables Jeff's response delay."), this);
  auto *keepHistoryExplanation = new ExplanationLabel(QTranslator::tr(
    "Enables keeping the message history after exiting the application."), this
  );
  save_and_close = new Button(QTranslator::tr("OK"), this);
  save_and_close->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  vert_box_lt->addWidget(delay);
  vert_box_lt->addWidget(delayExplanation);
  vert_box_lt->addWidget(box1);
  vert_box_lt->addWidget(keepHistory);
  vert_box_lt->addWidget(keepHistoryExplanation);
  vert_box_lt->addWidget(save_and_close);
  setLayout(vert_box_lt);
  connector();
  loadStates();
}

/*!
 * @fn Settings::connector
 * @brief Establishes communications for user interaction through the dialog box.
 */
void Settings::connector() {
  connect(delay, &QCheckBox::toggled, this, &Settings::delayChecked);
  connect(minDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::minDelayValueChanged);
  connect(maxDelay, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &Settings::maxDelayValueChanged);
  connect(save_and_close, &Button::clicked, this, &Settings::saveAndClose);
}

/*!
 * @fn Settings::loadStates
 * @brief Loads settings from file.
 */
void Settings::loadStates() {
  delay->setChecked(basis->read(basis->isDelayEnabledSt).toBool());
  delayChecked();
  keepHistory->setChecked(basis->read(basis->isKeepingEnabledSt).toBool());
  minDelay->setValue(basis->read(basis->minDelaySt).toInt());
  minDelayValueChanged(minDelay->value());
  maxDelay->setValue(basis->read(basis->maxDelaySt).toInt());
  maxDelayValueChanged(maxDelay->value());
}

/*!
 * @fn Settings::saveAndClose
 * @brief Saves Jeff's settings.
 */
void Settings::saveAndClose() {
  basis->write(basis->isDelayEnabledSt, delay->isChecked());
  basis->write(basis->isKeepingEnabledSt, keepHistory->isChecked());
  basis->write(basis->minDelaySt, minDelay->value());
  basis->write(basis->maxDelaySt, maxDelay->value());
  _m_handler->closePrisoner();
}

/*!
 * @fn Settings::minDelayValueChanged
 * @brief Changes maxDelay value, if minDelay value is bigger than that.
 * @param[in] value minDelay value
 */
void Settings::minDelayValueChanged(int value) {
  if (minDelay->value() > maxDelay->value())
    maxDelay->setValue(value);
}

/*!
 * @fn Settings::maxDelayValueChanged
 * @brief Changes minDelay value, if maxDelay value is smaller than that.
 * @param[in] value maxDelay value
 */
void Settings::maxDelayValueChanged(int value) {
  if (maxDelay->value() < minDelay->value())
    minDelay->setValue(value);
}
