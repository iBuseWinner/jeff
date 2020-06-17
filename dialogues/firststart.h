#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "dialogues/modal-handler.h"
#include "widgets/a_button.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QTranslator>
#include <QWidget>

/*!
 * Class: FirstStart.
 * Shows a quick start guide.
 */
class FirstStart : public QWidget {
public:
  // Functions:
  explicit FirstStart(QWidget *parent = nullptr,
                      ModalHandler *m_handler = nullptr);

private:
  Q_DISABLE_COPY(FirstStart)

  // Objects:
  ModalHandler *_m_handler = nullptr;
  inline static const QString objn = "firststart";
};

#endif // FIRSTSTART_H
