#ifndef AKIWAKE_MENUBAR_H
#define AKIWAKE_MENUBAR_H

#include <QMenuBar>
#include <QMenu>
#include <QWidget>




class AkiwakeMenuBar : public QMenuBar {
public:
  explicit AkiwakeMenuBar(QWidget *parent = nullptr);

private:

  Q_DISABLE_COPY(AkiwakeMenuBar)
    void openContainerManager();
};

#endif // AKIWAKE_MENUBAR_H
