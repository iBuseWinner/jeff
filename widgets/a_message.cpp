#include "a_message.h"

/*! Creates an AMessage. */
AMessage::AMessage() {
  setAttribute(Qt::WA_DeleteOnClose);
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
}

/*!
 * Argument: Message {shadow} [basis for the message].
 * Creates an AMessage based on {shadow}.
 */
AMessage::AMessage(Message shadow) {
  setAttribute(Qt::WA_DeleteOnClose);
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
  setShadow(shadow);
}

/*!
 * Argument: Message {_message} [basis for the message].
 * Sets {_message} into the AMessage.
 */
void AMessage::setShadow(Message _message) {
  if (_message.datetime.isNull())
    return;
  message = _message;
  setAuthor(message.aType);
  setMessageType(message.cType);
  // setTheme(sh.tType);
}

/*!
 * Argument: QWidget {*_widget} [widget to be showed].
 * Sets {_widget} into the AMessage.
 */
void AMessage::setWidget(QWidget *_widget) {
  if (widget)
    return;
  widget = _widget;
  connect(widget, &QWidget::destroyed, this, &AMessage::close);
  gridLayout->addWidget(widget);
}

/*!
 * Argument: enum Author {aType} [author].
 * Makes a message either left or right.
 */
void AMessage::setAuthor(Author aType) {
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
 * Argument: enum ContentType {cType} [content type].
 * Adjusts the message to the content type.
 */
void AMessage::setMessageType(ContentType cType) {
  switch (cType) {
  case 1:
    setupText(message.content);
    break;
  case 2:
    setupMarkdown(message.content);
    break;
  // case 3:
  //  setupPicture(sh.content);
  //  break;
  // case 4:
  //  setupFile(sh.content);
  //  break;
  case 5:
    setupWarning(message.content);
    break;
  case 6:
    setupError(message.content);
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
  QPair<QSpacerItem *, ABoard *> ws = makeLayout();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*! Customizes layout of message from user. */
void AMessage::setupUser() {
  QPair<QSpacerItem *, ABoard *> ws = makeLayout();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*!
 * Argument: QString {content}.
 * Displays plain text.
 */
void AMessage::setupText(const QString &content) {
  auto *label = new QLabel(content, this);
  widget = label;
  label->setObjectName("text");
  label->setTextFormat(Qt::PlainText);
  label->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                 Qt::TextSelectableByKeyboard);
  label->setFocusPolicy(Qt::NoFocus);
  gridLayout->addWidget(widget);
}

/*!
 * Argument: QString {content}.
 * Displays markdown text.
 */
void AMessage::setupMarkdown(const QString &content) {
  setupText(content);
  auto *label = static_cast<QLabel *>(widget);
  label->setTextFormat(Qt::RichText);
  label->setText(label->text().replace("\n", "<br>"));
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
  auto *board = static_cast<ABoard *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->wss);
  setupText(QString("Warning: " + content));
}

/*!
 * Argument: QString {content}.
 * Displays an error {content}.
 */
void AMessage::setupError(const QString &content) {
  auto *board = static_cast<ABoard *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->ess);
  setupText(QString("Error: " + content));
}

/*! Prepares AMessage for widget installation. */
void AMessage::prepareSetupWidget() {
  gridLayout->parentWidget()->setSizePolicy(QSizePolicy::Preferred,
                                            QSizePolicy::Fixed);
}

/*! Aligns text to the width of the window. */
void AMessage::alignTextToWindowWidth() {
  auto *label = qobject_cast<QLabel *>(widget);
  if (!label)
    return;
  QTextDocument textDocument;
  if (message.cType == ContentType::Markdown)
    textDocument.setHtml(label->text());
  else if (message.cType == ContentType::Text)
    textDocument.setPlainText(label->text());
  else
    return;
  if (textDocument.idealWidth() < maximalWidgetWidth)
    label->setWordWrap(false);
  else {
    label->setWordWrap(true);
    label->setFixedWidth(maximalWidgetWidth);
  }
}

/*!
 * Creates a spacer and an ABoard to adjust the layout.
 * Returns: QSpacerItem-ABoard pair.
 */
QPair<QSpacerItem *, ABoard *> AMessage::makeLayout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  auto *board = new ABoard(this);
  gridLayout = new QGridLayout();
  gridLayout->setSpacing(0);
  gridLayout->setMargin(standardMargin);
  board->setLayout(gridLayout);
  return QPair<QSpacerItem *, ABoard *>(spacer, board);
}
