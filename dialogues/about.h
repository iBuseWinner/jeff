#ifndef ABOUT_H
#define ABOUT_H

#include "core/settingsstore.h"
#include "dialogues/akiwake_dialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>

class About : public AkiwakeDialog {
public:
  explicit About(QWidget *parent = nullptr);
  ~About();
  void applyingSettings();
private:
  const QString sizeSt = "about/size";
};

#endif // ABOUT_H
