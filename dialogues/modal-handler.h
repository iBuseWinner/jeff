#ifndef MODALHANDLER_H
#define MODALHANDLER_H

#include <QObject>

/*!
 * @class ModalHandler
 * @brief The class that is responsible for the state of the prisoner.
 * @details Allows a prisoner, regardless of class, to tell AMessage that he is
 * closed. In this case, AMessage will be expected to be hidden from ADisplay.
 * @sa AMessage, ADisplay
 */
class ModalHandler : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(ModalHandler)
public:
  /*!
   * @brief The constructor.
   * @param[in,out] parent QObject parent
   * @param[in,out] prisoner widget whose state is controlled by ModalHandler
   */
  ModalHandler(QObject *parent = nullptr, QWidget *prisoner = nullptr)
      : QObject(parent), _prisoner(prisoner) {}
  /*! Closes the @a _prisoner. */
  void closePrisoner() { emit prisonerClosed(); }
  /*! Sets a @a _prisoner for ModalHandler. */
  void setPrisoner(QWidget *prisoner) { _prisoner = prisoner; }
  /*! @returns the @a _prisoner. */
  QWidget *getPrisoner() { return _prisoner; }

signals:
  /*!
   * @brief Says the @a _prisoner is closed.
   * @attention At this point, the custom widget on Display becomes zero size
   * and displays as an empty message. It must be removed with Display.
   */
  void prisonerClosed();

private:
  // Objects:
  QWidget *_prisoner = nullptr; /*!< Prisoner. */
};

#endif
