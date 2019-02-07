#ifndef AKIWAKE_BOARD_H
#define AKIWAKE_BOARD_H

#include <QFrame>
#include <QGridLayout>
#include <QWidget>

class AkiwakeBoard : public QFrame {
  Q_OBJECT
public:
  explicit AkiwakeBoard(QWidget *parent = nullptr);
  QWidget *centralWidget = new QWidget(this);

private:
  Q_DISABLE_COPY(AkiwakeBoard)
};

#endif // AKIWAKE_BOARD_H
