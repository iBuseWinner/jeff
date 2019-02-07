#include "akiwake_menubar.h"

AkiwakeMenuBar::AkiwakeMenuBar(AkiwakeLine *line, QWidget *parent)
    : QMenuBar(parent) {
  this->menuFile = this->addMenu("File");
  this->menuEdit = this->addMenu("Edit");
  this->menuTools = this->addMenu("Tools");
  QAction *containersMng = new QAction("Container Manager", this->menuFile);
  containersMng->setShortcut(Qt::CTRL + Qt::Key_M);
  QAction *sFile = new QAction("Save to file", this->menuFile);
  sFile->setShortcuts(QKeySequence::Save);
  QAction *del = new QAction("Delete", this->menuEdit);
  del->setShortcut(Qt::Key_Backspace);
  QAction *cut = new QAction("Cut", this->menuEdit);
  cut->setShortcuts(QKeySequence::Cut);
  QAction *copy = new QAction("Copy", this->menuEdit);
  copy->setShortcuts(QKeySequence::Copy);
  QAction *paste = new QAction("Paste", this->menuEdit);
  paste->setShortcuts(QKeySequence::Paste);
  QAction *selectAll = new QAction("Select all", this->menuEdit);
  selectAll->setShortcuts(QKeySequence::SelectAll);
  QAction *clearScr = new QAction("Clear screen", this->menuEdit);
  clearScr->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_D);
  this->fullScreen = new QAction("Full screen", this->menuTools);
  this->fullScreen->setShortcut(Qt::Key_F11);
  this->fullScreen->setCheckable(true);
  QAction *hideMenuBar = new QAction("Hide menu bar", this->menuTools);
  hideMenuBar->setShortcut(Qt::CTRL + Qt::Key_H);

  this->menuFile->addAction(containersMng);
  connect(containersMng, &QAction::triggered, this,
          &AkiwakeMenuBar::openContainerManager);
  this->menuFile->addAction(sFile);
  connect(sFile, &QAction::triggered, this, &AkiwakeMenuBar::saveToFile);
  this->menuFile->addSeparator();
  this->menuFile->addAction("&Exit", &QApplication::quit, Qt::ALT + Qt::Key_F4);
  this->menuEdit->addAction(clearScr);
  connect(clearScr, &QAction::triggered, this, &AkiwakeMenuBar::clearScreen);
  this->menuEdit->addSeparator();
  this->menuEdit->addAction(del);
  connect(del, &QAction::triggered, line->textLine,
          &AkiwakeLineEdit::backspace);
  this->menuEdit->addAction(cut);
  connect(cut, &QAction::triggered, line->textLine, &AkiwakeLineEdit::cut);
  this->menuEdit->addAction(copy);
  connect(copy, &QAction::triggered, line->textLine, &AkiwakeLineEdit::copy);
  this->menuEdit->addAction(paste);
  connect(paste, &QAction::triggered, line->textLine, &AkiwakeLineEdit::paste);
  this->menuEdit->addSeparator();
  this->menuEdit->addAction(selectAll);
  connect(selectAll, &QAction::triggered, line->textLine,
          &AkiwakeLineEdit::selectAll);
  this->menuTools->addAction(hideMenuBar);
  connect(hideMenuBar, &QAction::triggered, this, &AkiwakeMenuBar::hideThis);
  this->menuTools->addAction(this->fullScreen);
  connect(this->fullScreen, &QAction::triggered, this,
          &AkiwakeMenuBar::fScreen);
}

AkiwakeMenuBar::~AkiwakeMenuBar() {
  // Prevents memory leaks.
  delete this->menuFile;
  delete this->menuEdit;
  delete this->menuTools;
}

void AkiwakeMenuBar::openContainerManager() {
  auto *containersWindow = new Containers();
  containersWindow->exec();
  delete containersWindow;
}

void AkiwakeMenuBar::saveToFile() {}

void AkiwakeMenuBar::clearScreen() { emit clearScreenPressed(); }

void AkiwakeMenuBar::fScreen() { emit fullscreenModeChanged(); }

void AkiwakeMenuBar::hideThis() {
  if (this->isVisible())
    this->setVisible(false);
  else
    this->setVisible(true);
}
