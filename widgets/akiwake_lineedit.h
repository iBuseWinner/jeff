#ifndef AKIWAKE_LINEEDIT_H
#define AKIWAKE_LINEEDIT_H

#include <QFontDatabase>
#include <QLineEdit>
#include <QWidget>

class AkiwakeLineEdit : public QLineEdit {
public:
  explicit AkiwakeLineEdit(QWidget *parent = nullptr);
private:
  Q_DISABLE_COPY(AkiwakeLineEdit)
};

#endif // AKIWAKE_LINEEDIT_H
