#include "a_message.h"

/*
 * All short named objects and their explanations:
 * {pl} <- parent layout
 * {sh} <- shadow
 * {lt} <- layout
 * {stdm} <- standard margin
 * {w} <- widget
 * {ws} <- pair QSpacerItem-ABoard
 * {tl} <- text line
 */

/*! Creates an AMessage. */
AMessage::AMessage() {
  QHBoxLayout *pl = new QHBoxLayout();
  pl->setContentsMargins(0, 0, 0, stdm);
  pl->setSpacing(0);
  setLayout(pl);
}

/*!
 * Argument: message {shadow} [basis for the message].
 * Creates an AMessage based on {shadow}.
 */
AMessage::AMessage(message shadow) {
  QHBoxLayout *l = new QHBoxLayout();
  l->setContentsMargins(0, 0, 0, stdm);
  l->setSpacing(0);
  setLayout(l);
  setShadow(shadow);
}

/*!
 * Argument: message {shadow} [basis for the message].
 * Sets {shadow} into the AMessage.
 */
void AMessage::setShadow(message shadow) {
  if (shadow.datetime.isNull()) return;
  sh = shadow;
  setAuthor(sh.aType);
  setMessageType(sh.cType);
  // setTheme(sh.tType);
}

/*!
 * Argument: QWidget {*widget} [widget to be showed].
 * Sets {widget} into the AMessage.
 */
void AMessage::setWidget(QWidget *widget) {
  if (w != nullptr) return;
  w = widget;
  connect(w, &QWidget::destroyed, this, &AMessage::close);
  lt->addWidget(w);
}

/*!
 * Argument: enum eA {aType} [author].
 * Makes a message either left or right.
 */
void AMessage::setAuthor(eA aType) {
  switch (aType) {
    case 1:
      setupASW();
      break;
    case 2:
      setupUser();
      break;
    default:;
  }
}

/*!
 * Argument: enum eC {cType} [content type].
 * Adjusts the message to the content type.
 */
void AMessage::setMessageType(eC cType) {
  switch (cType) {
    case 1:
      setupText(sh.content);
      break;
    case 2:
      setupMarkdown(sh.content);
      break;
    // case 3:
    //  setupPicture(sh.content);
    //  break;
    // case 4:
    //  setupFile(sh.content);
    //  break;
    case 5:
      setupWarning(sh.content);
      break;
    case 6:
      setupError(sh.content);
      break;
    case 7:
      prepareSetupWidget();
      return;
    default:;
  }
  alignTextToWindowWidth();
}

/*!
 * Argument: enum eT {tType} [theme].
 * Sets the message colors.
 */
// void AMessage::setTheme(eT tType) {}

/*! Customizes layout of message from ASW. */
void AMessage::setupASW() {
  QPair<QSpacerItem *, ABoard *> ws = mkLt();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*! Customizes layout of message from user. */
void AMessage::setupUser() {
  QPair<QSpacerItem *, ABoard *> ws = mkLt();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*!
 * Argument: QString {content}.
 * Displays plain text.
 */
void AMessage::setupText(const QString &content) {
  auto *tl = new QLabel(content, this);
  w = tl;
  tl->setObjectName("text");
  tl->setTextFormat(Qt::PlainText);
  tl->setTextInteractionFlags(Qt::TextSelectableByMouse |
                              Qt::TextSelectableByKeyboard);
  tl->setFocusPolicy(Qt::NoFocus);
  lt->addWidget(w);
}

/*!
 * Argument: QString {content}.
 * Displays markdown text.
 */
void AMessage::setupMarkdown(const QString &content) {
  setupText(content);
  auto *tl = static_cast<QLabel *>(w);  // test
  tl->setTextFormat(Qt::RichText);
  tl->setText(tl->text().replace("\n", "<br>"));
}

/*!
 * Argument: QString {content}.
 * Displays a picture with the path {content}.
 */
// void AMessage::setupPicture(QString path) {}

/*!
 * Argument: QString {content}.
 * Displays a file with the path {content}.
 */
// void AMessage::setupFile(QString path) {}

/*!
 * Argument: QString {content}.
 * Displays a warning {content}.
 */
void AMessage::setupWarning(const QString &content) {
  setupText(QString("Warning: " + content));
}

/*!
 * Argument: QString {content}.
 * Displays an error {content}.
 */
void AMessage::setupError(const QString &content) {
  setupText(QString("Error: " + content));
}

/*! Prepares AMessage for widget installation. */
void AMessage::prepareSetupWidget() {
  lt->parentWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

/*! Aligns text to the width of the window. */
void AMessage::alignTextToWindowWidth() {
  auto *tl = static_cast<QLabel *>(w);
  if (tl == nullptr) return;
  QTextDocument td;
  if (sh.cType == eC::Markdown)
    td.setHtml(tl->text());
  else if (sh.cType == eC::Text)
    td.setPlainText(tl->text());
  else
    return;
  if (td.idealWidth() < mmw)
    tl->setWordWrap(false);
  else {
    tl->setWordWrap(true);
    tl->setFixedWidth(mmw);
  }
}

/*!
 * Creates a spacer and an ABoard to adjust the layout.
 * Returns: QSpacerItem-ABoard pair.
 */
QPair<QSpacerItem *, ABoard *> AMessage::mkLt() {
  auto *s = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                            QSizePolicy::Minimum);
  auto *b = new ABoard(this);
  lt = new QGridLayout();
  lt->setSpacing(0);
  lt->setMargin(stdm);
  b->setLayout(lt);
  return QPair<QSpacerItem *, ABoard *>(s, b);
}
