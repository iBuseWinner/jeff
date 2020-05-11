#include "a_menubar.h"

/*
 * All short named objects and their explanations:
 * {mf} <- menu File
 * {me} <- menu Edit
 * {mt} <- menu Tools
 * {mh} <- menu Help
 * {cm} <- container manager
 * {emh} <- export message history
 * {imh} <- import message history
 * {cmh} <- clear message history
 * {del} <- delete
 * {sel} <- select all
 * {hb} <- hide menubar
 * {st} <- settings
 */

/*!
 * Arguments: ALine {*line} [needed for the actions of the "Edit" menu],
 *            QWidget {*parent}.
 * Creates an AMenuBar.
 */
AMenuBar::AMenuBar(ALine *line, QWidget *parent)
    : QMenuBar(parent) {
  // File
  QMenu *mf = addMenu(tr("File"));
  QAction *cm = new QAction(tr("Source manager") + " (/sm)", mf);
  QAction *emh = new QAction(tr("Export message history"), mf);
  QAction *imh = new QAction(tr("Import message history"), mf);
  emm = new QAction(tr("Enable monologue mode") + " (/mm)", mf);
  emm->setCheckable(true);
  cm->setShortcut(Qt::CTRL + Qt::Key_M);
  emh->setShortcut(Qt::CTRL + Qt::Key_E);
  imh->setShortcut(Qt::CTRL + Qt::Key_I);
  emm->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_M);
  cm->setIcon(QIcon(":/arts/icons/16/database-manager.svg"));
  emh->setIcon(QIcon(":/arts/icons/16/document-export.svg"));
  imh->setIcon(QIcon(":/arts/icons/16/document-import.svg"));
  emm->setIcon(QIcon(":/arts/icons/16/monologue.svg"));
  mf->addAction(cm);
  mf->addAction(emm);
  mf->addSeparator();
  mf->addAction(emh);
  mf->addAction(imh);
  mf->addSeparator();
  mf->addAction(QIcon(":/arts/icons/16/application-exit.svg"), tr("&Exit"),
                &QApplication::quit, Qt::ALT + Qt::Key_F4);
  connect(cm, &QAction::triggered, this, &AMenuBar::openContainerManager);
  connect(emh, &QAction::triggered, this, &AMenuBar::exportMessageHistory);
  connect(imh, &QAction::triggered, this, &AMenuBar::importMessageHistory);
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
  cmh->setIcon(QIcon(":/arts/icons/16/clear-history.svg"));
  del->setIcon(QIcon(":/arts/icons/16/clear.svg"));
  cut->setIcon(QIcon(":/arts/icons/16/cut.svg"));
  copy->setIcon(QIcon(":/arts/icons/16/copy.svg"));
  paste->setIcon(QIcon(":/arts/icons/16/paste.svg"));
  sel->setIcon(QIcon(":/arts/icons/16/select-all.svg"));
  me->addAction(cmh);
  me->addSeparator();
  me->addAction(del);
  me->addAction(cut);
  me->addAction(copy);
  me->addAction(paste);
  me->addSeparator();
  me->addAction(sel);
  connect(cmh, &QAction::triggered, this, &AMenuBar::clearMessageHistory);
  connect(del, &QAction::triggered, line->lineEdit, &ALineEdit::backspace);
  connect(cut, &QAction::triggered, line->lineEdit, &ALineEdit::cut);
  connect(copy, &QAction::triggered, line->lineEdit, &ALineEdit::copy);
  connect(paste, &QAction::triggered, line->lineEdit, &ALineEdit::paste);
  connect(sel, &QAction::triggered, line->lineEdit, &ALineEdit::selectAll);
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
  connect(hb, &QAction::triggered, this, &AMenuBar::hideThis);
  connect(st, &QAction::triggered, this, &AMenuBar::openSettings);
  // Help
  QMenu *mh = addMenu(tr("Help"));
  QAction *about = new QAction(tr("About") + " (/about)", mh);
  about->setIcon(QIcon(":/arts/icons/16/help-about.svg"));
  mh->addAction(about);
  mh->addAction(QIcon(":/arts/icons/16/qt.svg"), tr("About Qt"),
                &QApplication::aboutQt);
  connect(about, &QAction::triggered, this, &AMenuBar::openAbout);
}
