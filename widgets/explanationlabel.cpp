#include "explanationlabel.h"

/*! @brief The constructor. */
ExplanationLabel::ExplanationLabel(const QString &text, QWidget *parent) : QLabel(parent) {
  setWordWrap(true);
  setTextFormat(Qt::RichText);
  setText("<i>" + text + "</i>");
}
