#include "akiwake_menubar.h"

AkiwakeMenuBar::AkiwakeMenuBar(AkiwakeLine *line, QWidget *parent)
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
          &AkiwakeMenuBar::openContainerManager);
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
  connect(clearScr, &QAction::triggered, this, &AkiwakeMenuBar::clearScreen);
  connect(del, &QAction::triggered, line->textLine,
          &AkiwakeLineEdit::backspace);
  connect(cut, &QAction::triggered, line->textLine, &AkiwakeLineEdit::cut);
  connect(copy, &QAction::triggered, line->textLine, &AkiwakeLineEdit::copy);
  connect(paste, &QAction::triggered, line->textLine, &AkiwakeLineEdit::paste);
  connect(selectAll, &QAction::triggered, line->textLine,
          &AkiwakeLineEdit::selectAll);
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
  connect(hideMenuBar, &QAction::triggered, this, &AkiwakeMenuBar::hideThis);
  connect(this->fullScreen, &QAction::triggered, this,
          &AkiwakeMenuBar::fScreen);
  // "Help" menu group...
  this->menuHelp = this->addMenu("Help");
  QAction *about = new QAction("About", this->menuHelp);
  this->menuHelp->addAction(about);
  this->menuHelp->addAction("About Qt", &QApplication::aboutQt);
  connect(about, &QAction::triggered, this, &AkiwakeMenuBar::openAbout);
}

AkiwakeMenuBar::~AkiwakeMenuBar() {
  // Prevents memory leaks.
  delete this->menuFile;
  delete this->menuEdit;
  delete this->menuTools;
  delete this->menuHelp;
}

void AkiwakeMenuBar::openContainerManager() {
  auto *containersWindow = new Containers();
  containersWindow->exec();
  delete containersWindow;
}

void AkiwakeMenuBar::openAbout() {
  auto *aboutWindow = new About();
  aboutWindow->exec();
  delete aboutWindow;
}

// void AkiwakeMenuBar::saveToFile() {}

void AkiwakeMenuBar::clearScreen() { emit clearScreenPressed(); }

void AkiwakeMenuBar::fScreen() { emit fullscreenModeChanged(); }

void AkiwakeMenuBar::hideThis() {
  if (this->isVisible())
    this->setVisible(false);
  else
    this->setVisible(true);
}
