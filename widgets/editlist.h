#ifndef EDIT_LIST_H
#define EDIT_LIST_H

#include <QGridLayout>
#include <QStringList>
#include <QVBoxLayout>
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"

/*! @class EditListWidget
 *  @brief TBD  */
class EditListWidget : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(EditListWidget)
public:
  // Functions described in `editlist.cpp`:
  EditListWidget(QWidget *parent = nullptr);
  QStringList get_list();
  void set_list(QStringList list);
  void set_add_btn_text(QString text);
  void set_rem_btn_text(QString text);
  void set_lineedit_placeholder_text(QString placeholder_text);
  void set_list_headers(QStringList headers);
  void clear();
  
private:
  // Objects:
  List list_widget;
  LineEdit line_edit;
  Button add_btn, remove_selected_btn;
  QWidget buttons_widget;
  QVBoxLayout buttons_widget_layout;
  QGridLayout main_layout;
  
  // Functions described in `editlist.cpp`:
  void add_element();
  void remove_element();
};

#endif
