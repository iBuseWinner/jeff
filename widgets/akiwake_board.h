#ifndef AKIWAKE_BOARD_H
#define AKIWAKE_BOARD_H

#include <QGridLayout>
#include <QWidget>

class AkiwakeBoard : public QWidget {
  Q_OBJECT
public:
  explicit AkiwakeBoard(QWidget *parent = nullptr);
  static QString borderTheme(qint8 Folder, qint8 ID);
  static QString borderTheme(qint8 Folder, qint8 ID, bool Swap);
  QWidget *centralWidget = new QWidget(this);
  QWidget *LH = new QWidget(this);
  QWidget *H = new QWidget(this);
  QWidget *RH = new QWidget(this);
  QWidget *L = new QWidget(this);
  QWidget *R = new QWidget(this);
  QWidget *LL1 = new QWidget(this);
  QWidget *L1 = new QWidget(this);
  QWidget *RL1 = new QWidget(this);

private:
  Q_DISABLE_COPY(AkiwakeBoard)
};

#endif // AKIWAKE_BOARD_H
