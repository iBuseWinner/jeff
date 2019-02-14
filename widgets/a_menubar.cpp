#include "a_menubar.h"

AMenuBar::AMenuBar(ALine *line, QWidget *parent)
    : QMenuBar(parent) {
  // Sub-menu bar in four stages:
  //    1) creating items;
  //    2) assigning of key sequences;
  //    3) adding items;
  //    4) connecting of signals and slots.
  // "File" menu group...
  this->menuFile = this->addMenu("File");
  QAction *containersMng = new QAction("Container Manager", this->menuFile);
  // QAction *sFile = new QAction("Save to file", this->menuFile);
  containersMng->setShortcut(Qt::CTRL + Qt::Key_M);
  // sFile->setShortcuts(QKeySequence::Save);
  this->menuFile->addAction(containersMng);
  // this->menuFile->addAction(sFile);
  this->menuFile->addSeparator();
  this->menuFile->addAction("&Exit", &QApplication::quit, Qt::ALT + Qt::Key_F4);
  connect(containersMng, &QAction::triggered, this,
          &AMenuBar::openContainerManager);
  // connect(sFile, &QAction::triggered, this, &AkiwakeMenuBar::saveToFile);
  // "Edit" menu group...
  this->menuEdit = this->addMenu("Edit");
  QAction *del = new QAction("Delete", this->menuEdit);
  QAction *cut = new QAction("Cut", this->menuEdit);
  QAction *copy = new QAction("Copy", this->menuEdit);
  QAction *paste = new QAction("Paste", this->menuEdit);
  QAction *selectAll = new QAction("Select all", this->menuEdit);
  QAction *clearScr = new QAction("Clear screen", this->menuEdit);
  del->setShortcut(Qt::Key_Backspace);
  cut->setShortcuts(QKeySequence::Cut);
  copy->setShortcuts(QKeySequence::Copy);
  paste->setShortcuts(QKeySequence::Paste);
  selectAll->setShortcuts(QKeySequence::SelectAll);
  clearScr->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_D);
  this->menuEdit->addAction(clearScr);
  this->menuEdit->addSeparator();
  this->menuEdit->addAction(del);
  this->menuEdit->addAction(cut);
  this->menuEdit->addAction(copy);
  this->menuEdit->addAction(paste);
  this->menuEdit->addSeparator();
  this->menuEdit->addAction(selectAll);
  connect(clearScr, &QAction::triggered, this, &AMenuBar::clearScreen);
  connect(del, &QAction::triggered, line->textLine,
          &ALineEdit::backspace);
  connect(cut, &QAction::triggered, line->textLine, &ALineEdit::cut);
  connect(copy, &QAction::triggered, line->textLine, &ALineEdit::copy);
  connect(paste, &QAction::triggered, line->textLine, &ALineEdit::paste);
  connect(selectAll, &QAction::triggered, line->textLine,
          &ALineEdit::selectAll);
  // "Tools" menu group...
  this->menuTools = this->addMenu("Tools");
  QAction *hideMenuBar = new QAction("Hide menu bar", this->menuTools);
  {
    this->fullScreen = new QAction("Full screen", this->menuTools);
    this->fullScreen->setCheckable(true);
  }
  this->fullScreen->setShortcut(Qt::Key_F11);
  hideMenuBar->setShortcut(Qt::CTRL + Qt::Key_H);
  this->menuTools->addAction(hideMenuBar);
  this->menuTools->addAction(this->fullScreen);
  connect(hideMenuBar, &QAction::triggered, this, &AMenuBar::hideThis);
  connect(this->fullScreen, &QAction::triggered, this,
          &AMenuBar::fScreen);
  // "Help" menu group...
  this->menuHelp = this->addMenu("Help");
  QAction *about = new QAction("About", this->menuHelp);
  this->menuHelp->addAction(about);
  this->menuHelp->addAction("About Qt", &QApplication::aboutQt);
  connect(about, &QAction::triggered, this, &AMenuBar::openAbout);
}

AMenuBar::~AMenuBar() {
  // Prevents memory leaks.
  delete this->menuFile;
  delete this->menuEdit;
  delete this->menuTools;
  delete this->menuHelp;
}

void AMenuBar::openContainerManager() { emit contManTriggered(); }

void AMenuBar::openAbout() { emit aboutTriggered(); }

void AMenuBar::saveToFile() { emit saveToFileTriggered(); }

void AMenuBar::clearScreen() { emit clearScreenTriggered(); }

void AMenuBar::fScreen() { emit fullscreenModeChanged(); }

void AMenuBar::hideThis() {
  if (this->isVisible())
    this->setVisible(false);
  else
    this->setVisible(true);
}
