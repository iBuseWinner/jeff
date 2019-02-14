#ifndef A_LINEEDIT_H
#define A_LINEEDIT_H

#include <QFontDatabase>
#include <QLineEdit>
#include <QWidget>

class ALineEdit : public QLineEdit {
public:
  ALineEdit(QWidget *parent = nullptr);
private:
  Q_DISABLE_COPY(ALineEdit)
};

#endif // A_LINEEDIT_H
