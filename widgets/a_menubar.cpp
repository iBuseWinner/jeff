#include "a_menubar.h"

AMenuBar::AMenuBar(ALine *line, QWidget *parent) : QMenuBar(parent) {
  // Sub-menu bar in four stages:
  //    1) creating items;
  //    2) assigning of key sequences;
  //    3) setting icons
  //    4) adding items;
  //    5) connecting of signals and slots.
  // "File" menu group...
  menuFile = this->addMenu("File");
  QAction *containersMng = new QAction("Container manager", menuFile);
  // QAction *sFile = new QAction("Save to file", menuFile);
  containersMng->setShortcut(Qt::CTRL + Qt::Key_M);
  // sFile->setShortcuts(QKeySequence::Save);
  containersMng->setIcon(QIcon(":/arts/icons/16/database-manager.svg"));
  menuFile->addAction(containersMng);
  // menuFile->addAction(sFile);
  menuFile->addSeparator();
  menuFile->addAction(QIcon(":/arts/icons/16/application-exit.svg"), "&Exit",
                      &QApplication::quit, Qt::ALT + Qt::Key_F4);
  connect(containersMng, &QAction::triggered, this,
          &AMenuBar::openContainerManager);
  // connect(sFile, &QAction::triggered, this, &AkiwakeMenuBar::saveToFile);
  // "Edit" menu group...
  menuEdit = this->addMenu("Edit");
  QAction *clearMsgHist = new QAction("Clear message history", menuEdit);
  QAction *del = new QAction("Delete", menuEdit);
  QAction *cut = new QAction("Cut", menuEdit);
  QAction *copy = new QAction("Copy", menuEdit);
  QAction *paste = new QAction("Paste", menuEdit);
  QAction *selectAll = new QAction("Select all", menuEdit);
  clearMsgHist->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_D);
  del->setShortcut(Qt::Key_Backspace);
  cut->setShortcuts(QKeySequence::Cut);
  copy->setShortcuts(QKeySequence::Copy);
  paste->setShortcuts(QKeySequence::Paste);
  selectAll->setShortcuts(QKeySequence::SelectAll);
  clearMsgHist->setIcon(QIcon(":/arts/icons/16/clear-history.svg"));
  del->setIcon(QIcon(":/arts/icons/16/clear.svg"));
  cut->setIcon(QIcon(":/arts/icons/16/cut.svg"));
  copy->setIcon(QIcon(":/arts/icons/16/copy.svg"));
  paste->setIcon(QIcon(":/arts/icons/16/paste.svg"));
  selectAll->setIcon(QIcon(":/arts/icons/16/select-all.svg"));
  menuEdit->addAction(clearMsgHist);
  menuEdit->addSeparator();
  menuEdit->addAction(del);
  menuEdit->addAction(cut);
  menuEdit->addAction(copy);
  menuEdit->addAction(paste);
  menuEdit->addSeparator();
  menuEdit->addAction(selectAll);
  connect(clearMsgHist, &QAction::triggered, this, &AMenuBar::clearScreen);
  connect(del, &QAction::triggered, line->textLine, &ALineEdit::backspace);
  connect(cut, &QAction::triggered, line->textLine, &ALineEdit::cut);
  connect(copy, &QAction::triggered, line->textLine, &ALineEdit::copy);
  connect(paste, &QAction::triggered, line->textLine, &ALineEdit::paste);
  connect(selectAll, &QAction::triggered, line->textLine,
          &ALineEdit::selectAll);
  // "Tools" menu group...
  this->menuTools = this->addMenu("Tools");
  QAction *hideMenuBar = new QAction("Hide menu bar", this->menuTools);
  {
    fullScreen = new QAction("Full screen", this->menuTools);
    fullScreen->setCheckable(true);
  }
  hideMenuBar->setShortcut(Qt::CTRL + Qt::Key_H);
  fullScreen->setShortcut(Qt::Key_F11);
  hideMenuBar->setIcon(QIcon(":/arts/icons/16/show-menu.svg"));
  fullScreen->setIcon(QIcon(":/arts/icons/16/view-fullscreen.svg"));
  menuTools->addAction(hideMenuBar);
  menuTools->addAction(fullScreen);
  connect(hideMenuBar, &QAction::triggered, this, &AMenuBar::hideThis);
  connect(fullScreen, &QAction::triggered, this, &AMenuBar::fScreen);
  // "Help" menu group...
  menuHelp = this->addMenu("Help");
  QAction *about = new QAction("About", menuHelp);
  about->setIcon(QIcon(":/arts/icons/16/help-about.svg"));
  menuHelp->addAction(about);
  menuHelp->addAction(QIcon(":/arts/icons/16/qt.svg"), "About Qt",
                      &QApplication::aboutQt);
  connect(about, &QAction::triggered, this, &AMenuBar::openAbout);
}

AMenuBar::~AMenuBar() {
  // Prevents memory leaks.
  delete menuFile;
  delete menuEdit;
  delete menuTools;
  delete menuHelp;
}

void AMenuBar::openContainerManager() { emit contManTriggered(); }

void AMenuBar::openAbout() { emit aboutTriggered(); }

void AMenuBar::saveToFile() { emit saveToFileTriggered(); }

void AMenuBar::clearScreen() { emit clearScreenTriggered(); }

void AMenuBar::fScreen() { emit fullscreenModeChanged(); }

void AMenuBar::hideThis() { this->setVisible(!this->isVisible()); }
