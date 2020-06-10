#ifndef A_EXPLANATIONLABEL_H
#define A_EXPLANATIONLABEL_H

#include <QLabel>

class AExplanationLabel : public QLabel
{
public:
  AExplanationLabel(const QString &text = "", QWidget *parent = nullptr);
};

#endif // A_EXPLANATIONLABEL_H
