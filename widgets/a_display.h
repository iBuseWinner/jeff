#ifndef A_DISPLAY_H
#define A_DISPLAY_H

#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

class ADisplay : public QScrollArea {
  Q_OBJECT
public:
  explicit ADisplay(QWidget *parent = nullptr);
  void connector();
  void start();
  QVBoxLayout *layout = nullptr;
  bool scrollEnabled = true;

private:
  Q_DISABLE_COPY(ADisplay)
  void scrollDown(int min, int max);
  void scrollTumbler(int value);
};

#endif // A_DISPLAY_H
