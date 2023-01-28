#ifndef GRID_LT_H
#define GRID_LT_H

#include <QLayoutItem>
#include <QGridLayout>
#include <QWidget>

/*! @class GridLt
 *  @brief Grid for widgets in reactive paradigm.  */
class GridLt : public QGridLayout {
  Q_OBJECT
  Q_DISABLE_COPY(GridLt)
public:
  /*! @brief The constructor. */
  GridLt(QWidget *parent = nullptr) : QGridLayout(parent) { setMargin(0); }
  static GridLt *another() { return new GridLt(); }
  GridLt *spacing(int spacing = 0) { setSpacing(spacing); return this; }
  GridLt *margin(int margin = 0) { setMargin(margin); return this; }
  GridLt *rem(QWidget *w) { removeWidget(w); return this; }
  GridLt *addw(
    QWidget *w, int r = 0, int c = 0, int rs = 1, int cs = 1, Qt::Alignment a = Qt::Alignment()) {
    addWidget(w, r, c, rs, cs, a);
    return this;
  }
  GridLt *addi(QLayoutItem *i, int r = 0, int c = 0, int rs = 1, int cs = 1) {
    addItem(i, r, c, rs, cs);
    return this;
  }
  GridLt *addlt(QLayout *lt, int r = 0, int c = 0, int rs = 1, int cs = 1) {
    addLayout(lt, r, c, rs, cs);
    return this;
  }
  GridLt *cmargins(int l = 0, int t = 0, int r = 0, int b = 0) {
    setContentsMargins(l, t, r, b);
    return this;
  }
};

#endif
