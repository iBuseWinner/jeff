#ifndef EXPLANATIONLABEL_H
#define EXPLANATIONLABEL_H

#include <QLabel>

/*!
 * @class ExplanationLabel
 * @brief Label explaining settings.
 */
class ExplanationLabel : public QLabel {
  Q_OBJECT
public:
  // Functions described in 'explanationlabel.cpp':
  explicit ExplanationLabel(const QString &text = "",
                             QWidget *parent = nullptr);
};

#endif
