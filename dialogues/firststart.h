#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "dialogues/akiwake_dialog.h"
#include <QLabel>
#include <QScrollArea>
#include <QSpacerItem>

class FirstStart : public AkiwakeDialog {
public:
  explicit FirstStart(QWidget *parent = nullptr);
};

#endif // FIRSTSTART_H
