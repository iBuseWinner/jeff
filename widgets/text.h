#ifndef TEXT_H
#define TEXT_H

#include <QLabel>

/*! @class Text
 *  @brief TBD  */
class Text : public QLabel {
  Q_OBJECT
  Q_DISABLE_COPY(Text)
public:
  // Functions described in `text.cpp`:
  Text(QString text = "", QWidget *parent = nullptr);
};

#endif
