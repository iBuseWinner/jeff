#ifndef MODALHANDLER_H
#define MODALHANDLER_H

#include <QObject>

class ModalHandler : public QObject {
  Q_OBJECT
public:
  ModalHandler(QObject *parent = nullptr, QWidget *prisoner = nullptr)
      : QObject(parent), _prisoner(prisoner) {}
  void closePrisoner() { emit prisonerClosed(_prisoner); }
  void setPrisoner(QWidget *prisoner) { _prisoner = prisoner; }
  QWidget *getPrisoner() { return _prisoner; }

signals:
  QWidget *prisonerClosed(QWidget *prisoner);

private:
  Q_DISABLE_COPY(ModalHandler)
  QWidget *_prisoner = nullptr;
};

#endif // MODALHANDLER_H
