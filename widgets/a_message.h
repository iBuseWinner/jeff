#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include "widgets/a_board.h"
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWidget>

class AMessage : public QWidget {
  Q_OBJECT
public:
  enum AT { undefAT, ASW, User };
  enum MT { undefMT, Text, HTML, Widget, Picture, File };
  enum T { undefTT, Light, Dark, Red, Green, Blue, Yellow };
  AMessage(QWidget *parent = nullptr);
  void setAuthor(AT Author);
  void setMessageType(MT Type, QString Content);
  void setMessageType(MT Type, QWidget *Content);
  void setTheme(T Theme);
  enum AT returnAuthor() { return this->m_author; }
  enum MT returnMessageType() { return this->m_contentType; }
  enum T returnTheme() { return this->m_theme; }
  QString returnText();

private:
  Q_DISABLE_COPY(AMessage)
  AT m_author = AT::undefAT;
  MT m_contentType = MT::undefMT;
  T m_theme = T::undefTT;
  QHBoxLayout *entireLayout = new QHBoxLayout();
  ABoard *board = new ABoard(this);
  QString m_content = "";
  void createStdLayout_asw();
  void createStdLayout_user();
  // void setupMessage_plain(QString text);
  void setupMessage_html(const QString& text);
  void setupMessage_widget(QWidget *widget);
  // void setupMessage_pict(QString path);
  // void setupMessage_file(QString path);
};

#endif // A_MESSAGE_H
