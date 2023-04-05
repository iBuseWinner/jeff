#ifndef EXPLANATIONLABEL_H
#define EXPLANATIONLABEL_H

#include <QLabel>

/*! @class ExplanationLabel
 *  @brief Label explaining settings.  */
class ExplanationLabel : public QLabel {
  Q_OBJECT
  Q_DISABLE_COPY(ExplanationLabel)
public:
  // Functions described in `explanationlabel.cpp`:
  ExplanationLabel(const QString &text = "", QWidget *parent = nullptr);
  void setText(const QString &text);
};

#endif
