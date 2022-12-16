#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "widgets/styling.h"
#include <QComboBox>

/*! @class ComboBox
 *  @brief Widget with many options.  */
class ComboBox : public QComboBox {
  Q_OBJECT
  Q_DISABLE_COPY(ComboBox)
public:
  // Functions described in `combobox.h`:
  ComboBox(QWidget *parent = nullptr);
};

#endif
