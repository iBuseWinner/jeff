#include "a_explanationlabel.h"

/*!
 * @fn AExplanationLabel::AExplanationLabel
 * @brief The constructor.
 * @param[in] text text explaining the setting
 * \param[in,out] parent QObject parent
 */
AExplanationLabel::AExplanationLabel(const QString &text, QWidget *parent)
    : QLabel(parent) {
  setWordWrap(true);
  setTextFormat(Qt::RichText);
  setText("<i>" + text + "</i>");
}
