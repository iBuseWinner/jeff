#include "a_explanationlabel.h"

/*!
 * Arguments: QString {text},
 *            QWidget {*parent}.
 * Creates an AExplanationLabel.
 */
AExplanationLabel::AExplanationLabel(const QString &text, QWidget *parent)
    : QLabel(parent) {
  setWordWrap(true);
  setTextFormat(Qt::RichText);
  setText("<i>" + text + "</i>");
}
