#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QTranslator>
#include <QWidget>

/*!
 * @class FirstStart
 * @brief Shows a quick start guide.
 * @sa ModalHandler
 */
class FirstStart : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(FirstStart)
public:
  // Functions described in `firststart.cpp`:
  FirstStart(QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  ModalHandler *_m_handler = nullptr;

  // Constants:
  inline static const QString object_name = "firststart";
  static const int fixed_width = 400;
};

#endif
