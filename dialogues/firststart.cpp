#include "firststart.h"

/*
 * All short named objects and their explanations:
 * {lt} <- layout
 * {sp} <- spacer
 * {objn} <- object name
 */

/*!
 * Argument: QWidget {*parent}.
 * Constructs and prepares First Start.
 */
FirstStart::FirstStart(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(objn);
  auto *lt = new QVBoxLayout();
  auto *title = new QLabel(
      "<font size=\"6\">" + QTranslator::tr("Associative System") + "</font>",
      this);
  auto *startText = new QLabel(
      "<font size=\"3\">" +
          QTranslator::tr(
              "This is the first start of ASW. Before you start working:") +
          "<br>" + QTranslator::tr("1) go to the Container Manager...") +
          "<p><img src=\":/arts/shots/menufile.png\"></p>" +
          QTranslator::tr("2) add databases...") +
          "<p><img src=\":/arts/shots/addcontainer.png\"></p>" +
          QTranslator::tr("And now you've done.") + "</font>",
      this);
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                     Qt::LinksAccessibleByMouse);
  auto *bottomLine = new QWidget(this);
  auto *closeBtn = new AButton(QTranslator::tr("Close"), bottomLine);
  auto *bottomLayout = new QHBoxLayout();
  bottomLayout->setMargin(0);
  bottomLayout->setSpacing(0);
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  bottomLayout->addItem(sp);
  bottomLayout->addWidget(closeBtn);
  bottomLine->setLayout(bottomLayout);
  connect(closeBtn, &AButton::clicked, this, &QWidget::close);
  lt->addWidget(title);
  lt->addWidget(startText);
  lt->addWidget(bottomLine);
  setLayout(lt);
}
