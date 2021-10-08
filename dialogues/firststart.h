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
public:
  // Functions described in 'firststart.cpp':
  explicit FirstStart(QWidget *parent = nullptr,
                      ModalHandler *m_handler = nullptr);

private:
  Q_DISABLE_COPY(FirstStart)

  // Objects:
  ModalHandler *_m_handler = nullptr;

  // Constants:
  inline static const QString object_name = "firststart";
};

#endif
