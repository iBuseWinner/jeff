#ifndef EDIT_LIST_H
#define EDIT_LIST_H

#include <QGridLayout>
#include <QStringList>
#include <QVBoxLayout>
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"

/*! @class EditList
 *  @brief Editable list of elements.  */
class EditList : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(EditList)
public:
  // Functions described in `editlist.cpp`:
  EditList(QWidget *parent = nullptr);
  QStringList get_list();
  void append(QStringList list);
  void set_add_btn_text(QString text);
  void set_rem_btn_text(QString text);
  void set_lineedit_placeholder_text(QString placeholder_text);
  void set_list_headers(QStringList headers);
  void clear();
  bool is_empty();
  
private:
  // Objects:
  List *list_widget;
  LineEdit *line_edit;
  Button *add_btn, *remove_selected_btn;
  
  // Functions described in `editlist.cpp`:
  void add_element();
  void remove_element();
};

#endif
