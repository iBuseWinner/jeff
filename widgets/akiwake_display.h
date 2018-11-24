#ifndef AKIWAKE_DISPLAY_H
#define AKIWAKE_DISPLAY_H

#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

class AkiwakeDisplay : public QScrollArea {
  Q_OBJECT
public:
  explicit AkiwakeDisplay(QWidget *parent = nullptr);
  void connector();
  QVBoxLayout *layout;
  bool scrollEnabled = true;

private:
  Q_DISABLE_COPY(AkiwakeDisplay)
  void scrollDown(int min, int max);
  void scrollTumbler(int value);
};

#endif // AKIWAKE_DISPLAY_H
