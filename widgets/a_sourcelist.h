#ifndef A_SOURCELIST_H
#define A_SOURCELIST_H

#include <QTreeWidget>

/*!
 * @class ASourceList
 * @brief Source list widget.
 */
class ASourceList : public QTreeWidget {
  Q_OBJECT
public:
  // Functions described in 'a_sourcelist.cpp':
  explicit ASourceList(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(ASourceList)
};

#endif // A_SOURCELIST_H
