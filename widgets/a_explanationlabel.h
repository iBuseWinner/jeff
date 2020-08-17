#ifndef A_EXPLANATIONLABEL_H
#define A_EXPLANATIONLABEL_H

#include <QLabel>

/*!
 * @class AExplanationLabel
 * @brief Label explaining settings.
 */
class AExplanationLabel : public QLabel {
  Q_OBJECT
public:
  // Functions described in 'a_explanationlabel.cpp':
  explicit AExplanationLabel(const QString &text = "",
                             QWidget *parent = nullptr);
};

#endif // A_EXPLANATIONLABEL_H
