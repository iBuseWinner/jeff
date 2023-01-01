#ifndef PAIR_WIDGET_H
#define PAIR_WIDGET_H

#include <QHBoxLayout>
#include <QWidget>

/*! @class PairWidget
 *  @brief A simple wrapper for two widgets.  */
class PairWidget : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PairWidget)
public:
  // Functions described in `pairwidget.cpp`:
  PairWidget(QWidget *w1, QWidget *w2, QWidget *parent = nullptr);
};

#endif
