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
  QWidget *centralWidget = new QWidget();
  QWidget *LH = new QWidget();
  QWidget *H = new QWidget();
  QWidget *RH = new QWidget();
  QWidget *L = new QWidget();
  QWidget *R = new QWidget();
  QWidget *LL1 = new QWidget();
  QWidget *L1 = new QWidget();
  QWidget *RL1 = new QWidget();

private:
  Q_DISABLE_COPY(AkiwakeBoard)
};

#endif // AKIWAKE_BOARD_H
