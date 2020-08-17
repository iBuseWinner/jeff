#ifndef EDITSOURCEDIALOG_H
#define EDITSOURCEDIALOG_H

#include "core/basis.h"
#include "widgets/a_expressionlist.h"
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QWidget>

/*!
 * @class EditSourceDialog
 * @brief Edits databases and allows users to change the reaction of ASW.
 * @sa Basis, AExpressionList
 */
class EditSourceDialog : public QWidget {
  Q_OBJECT
public:
  // Functions described in 'edit-source.cpp':
  explicit EditSourceDialog(Basis *_basis, QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(EditSourceDialog)

  // Objects:
  Basis *basis = nullptr;
  AExpressionList *activators = nullptr, *curr = nullptr, *reagents = nullptr;
};

#endif // EDITSOURCEDIALOG_H
