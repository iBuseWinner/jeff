#include "explanationlabel.h"

/*! @brief The constructor. */
ExplanationLabel::ExplanationLabel(const QString &text, QWidget *parent) : QLabel(parent) {
  setWordWrap(true);
  setTextFormat(Qt::RichText);
  setText(text);
}

/*! @brief Makes text italic. */
void ExplanationLabel::setText(const QString &text) { QLabel::setText("<i>" + text + "</i>"); }
