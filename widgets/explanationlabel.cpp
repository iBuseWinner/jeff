#include "explanationlabel.h"

/*!
 * @fn ExplanationLabel::ExplanationLabel
 * @brief The constructor.
 * @param[in] text text explaining the setting
 * \param[in,out] parent QObject parent
 */
ExplanationLabel::ExplanationLabel(const QString &text, QWidget *parent) : QLabel(parent) {
  setWordWrap(true);
  setTextFormat(Qt::RichText);
  setText("<i>" + text + "</i>");
}
