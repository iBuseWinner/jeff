#include "menubar.h"

/*! All short named objects and their explanations:
 * @a mf <- menu File <- @a MenuBar::MenuBar
 * @a me <- menu Edit <- @a MenuBar::MenuBar
 * @a mt <- menu Tools <- @a MenuBar::MenuBar
 * @a mh <- menu Help <- @a MenuBar::MenuBar
 * @a sm <- source manager <- @a MenuBar::MenuBar
 * @a emh <- export message history <- @a MenuBar::MenuBar
 * @a imh <- import message history <- @a MenuBar::MenuBar
 * @a cmh <- clear message history <- @a MenuBar::MenuBar
 * @a del <- delete <- @a MenuBar::MenuBar
 * @a sel <- select all <- @a MenuBar::MenuBar
 * @a hb <- hide menubar <- @a MenuBar::MenuBar
 * @a st <- settings <- @a MenuBar::MenuBar
 */

/*!
 * @fn MenuBar::MenuBar
 * @brief The constructor.
 * @param[in,out] line reference to the Line instance
 * @param[in,out] parent QObject parent
 */
MenuBar::MenuBar(Line *line, QWidget *parent) : QMenuBar(parent) {
  // File
  QMenu *mf = addMenu(tr("File"));
  QAction *sm = new QAction(tr("Source manager") + " (/sm)", mf);
  QAction *emh = new QAction(tr("Export message history"), mf);
  QAction *imh = new QAction(tr("Import message history"), mf);
  emm = new QAction(tr("Enable monologue mode") + " (/mm)", mf);
  emm->setCheckable(true);
  sm->setShortcut(Qt::CTRL + Qt::Key_M);
  emh->setShortcut(Qt::CTRL + Qt::Key_E);
  imh->setShortcut(Qt::CTRL + Qt::Key_I);
  emm->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_M);
#ifdef Q_OS_UNIX
  sm->setIcon(QIcon::fromTheme("network-server-database"));
  emh->setIcon(QIcon::fromTheme("document-export.svg"));
  imh->setIcon(QIcon::fromTheme("document-import.svg"));
  emm->setIcon(QIcon::fromTheme("user-group-properties"));
#else
  sm->setIcon(QIcon(":/arts/icons/16/database-manager.svg"));
  emh->setIcon(QIcon(":/arts/icons/16/document-export.svg"));
  imh->setIcon(QIcon(":/arts/icons/16/document-import.svg"));
  emm->setIcon(QIcon(":/arts/icons/16/monologue.svg"));
#endif
  mf->addAction(sm);
  mf->addAction(emm);
  mf->addSeparator();
  mf->addAction(emh);
  mf->addAction(imh);
  mf->addSeparator();
  mf->addAction(
#ifdef Q_OS_UNIX
      QIcon::fromTheme("application-exit")
#else
      QIcon(":/arts/icons/16/application-exit.svg")
#endif
          ,
      tr("&Exit"), &QApplication::quit, Qt::ALT + Qt::Key_F4);
  connect(sm, &QAction::triggered, this, [this] { emit sourcesTriggered(); });
  connect(emh, &QAction::triggered, this, [this] { emit exportTriggered(); });
  connect(imh, &QAction::triggered, this, [this] { emit importTriggered(); });
  // Edit
  QMenu *me = addMenu(tr("Edit"));
  QAction *cmh = new QAction(tr("Clear message history"), me);
  QAction *del = new QAction(tr("Delete"), me);
  QAction *cut = new QAction(tr("Cut"), me);
  QAction *copy = new QAction(tr("Copy"), me);
  QAction *paste = new QAction(tr("Paste"), me);
  QAction *sel = new QAction(tr("Select all"), me);
  cmh->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_D);
  del->setShortcut(Qt::Key_Backspace);
  cut->setShortcuts(QKeySequence::Cut);
  copy->setShortcuts(QKeySequence::Copy);
  paste->setShortcuts(QKeySequence::Paste);
  sel->setShortcuts(QKeySequence::SelectAll);
#ifdef Q_OS_UNIX
  cmh->setIcon(QIcon::fromTheme("edit-clear-history"));
#else
  cmh->setIcon(QIcon(":/arts/icons/16/clear-history.svg"));
  del->setIcon(QIcon(":/arts/icons/16/clear.svg"));
  cut->setIcon(QIcon(":/arts/icons/16/cut.svg"));
  copy->setIcon(QIcon(":/arts/icons/16/copy.svg"));
  paste->setIcon(QIcon(":/arts/icons/16/paste.svg"));
  sel->setIcon(QIcon(":/arts/icons/16/select-all.svg"));
#endif
  me->addAction(cmh);
  me->addSeparator();
  me->addAction(del);
  me->addAction(cut);
  me->addAction(copy);
  me->addAction(paste);
  me->addSeparator();
  me->addAction(sel);
  connect(cmh, &QAction::triggered, this,
          [this] { emit clearHistoryTriggered(); });
  connect(del, &QAction::triggered, line->lineEdit, &LineEdit::backspace);
  connect(cut, &QAction::triggered, line->lineEdit, &LineEdit::cut);
  connect(copy, &QAction::triggered, line->lineEdit, &LineEdit::copy);
  connect(paste, &QAction::triggered, line->lineEdit, &LineEdit::paste);
  connect(sel, &QAction::triggered, line->lineEdit, &LineEdit::selectAll);
  // Tools
  QMenu *mt = addMenu(tr("Tools"));
  QAction *hb = new QAction(tr("Hide menubar"), mt);
  fullScreenAction = new QAction(tr("Full screen"), mt);
  fullScreenAction->setCheckable(true);
  QAction *st = new QAction(tr("Settings...") + " (/settings)", mt);
  hb->setShortcut(Qt::CTRL + Qt::Key_H);
  fullScreenAction->setShortcut(Qt::Key_F11);
  st->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Comma);
  hb->setIcon(QIcon(":/arts/icons/16/show-menu.svg"));
  fullScreenAction->setIcon(QIcon(":/arts/icons/16/view-fullscreen.svg"));
  st->setIcon(QIcon(":/arts/icons/16/configure.svg"));
  mt->addAction(hb);
  mt->addAction(fullScreenAction);
  mt->addSeparator();
  mt->addAction(st);
  connect(hb, &QAction::triggered, this, [this] { setVisible(!isVisible()); });
  connect(st, &QAction::triggered, this, [this] { emit settingsTriggered(); });
  // Help
  QMenu *mh = addMenu(tr("Help"));
  QAction *about = new QAction(tr("About") + " (/about)", mh);
  about->setIcon(QIcon(":/arts/icons/16/help-about.svg"));
  mh->addAction(about);
  mh->addAction(QIcon(":/arts/icons/16/qt.svg"), tr("About Qt"),
                &QApplication::aboutQt);
  connect(about, &QAction::triggered, this, [this] { emit aboutTriggered(); });
}
