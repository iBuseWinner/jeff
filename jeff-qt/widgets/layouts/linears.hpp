#ifndef LINEAR_LTS_H
#define LINEAR_LTS_H

#include <QHBoxLayout>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QWidget>

/*! @class VLineLt
 *  @brief Vertical layout for widgets in a reactive paradigm.  */
class VLineLt : public QVBoxLayout {
  Q_OBJECT
  Q_DISABLE_COPY(VLineLt)
public:
  VLineLt(QWidget *parent = nullptr) : QVBoxLayout(parent) { setMargin(0); }
  static VLineLt *another() { return new VLineLt(); }
  VLineLt *rem(QWidget *w) { removeWidget(w); return this; }
  VLineLt *addw(QWidget *w) { addWidget(w); return this; }
  VLineLt *addi(QLayoutItem *i) { addItem(i); return this; }
  VLineLt *addlt(QLayout *lt) { addLayout(lt); return this; }
  VLineLt *spacing(int spacing = 0) { setSpacing(spacing); return this; }
  VLineLt *margin(int margin = 0) { setMargin(margin); return this; }
  VLineLt *cmargins(int left = 0, int top = 0, int right = 0, int bottom = 0) {
    setContentsMargins(left, top, right, bottom);
    return this;
  }
};

/*! @class HLineLt
 *  @brief Horizontal layout for widgets in a reactive paradigm.  */
class HLineLt : public QHBoxLayout {
  Q_OBJECT
  Q_DISABLE_COPY(HLineLt)
public:
  HLineLt(QWidget *parent = nullptr) : QHBoxLayout(parent) { setMargin(0); }
  static HLineLt *another() { return new HLineLt(); }
  HLineLt *rem(QWidget *w) { removeWidget(w); return this; }
  HLineLt *addw(QWidget *w) { addWidget(w); return this; }
  HLineLt *addi(QLayoutItem *i) { addItem(i); return this; }
  HLineLt *addlt(QLayout *lt) { addLayout(lt); return this; }
  HLineLt *spacing(int spacing = 0) { setSpacing(spacing); return this; }
  HLineLt *margin(int margin = 0) { setMargin(margin); return this; }
  HLineLt *cmargins(int left = 0, int top = 0, int right = 0, int bottom = 0) {
    setContentsMargins(left, top, right, bottom);
    return this;
  }
};

#endif
