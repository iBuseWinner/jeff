#include "akiwake_menubar.h"
AkiwakeMenuBar::AkiwakeMenuBar(AkiwakeLine *line,QWidget *parent) : QMenuBar(parent) {
  QMenu *menuFile = this->addMenu("File");
  QMenu *menuEdit = this->addMenu("Edit");
  QMenu *menuTools = this->addMenu("Tools");
  QAction *containersMng = new QAction("Container Manager", this);
  containersMng->setShortcut(Qt::CTRL + Qt::Key_M);
  QAction *sFile = new QAction ("Save to file",this);
  sFile->setShortcuts(QKeySequence::Save);
  QAction *del = new QAction ("Delete",this);
  del->setShortcut(Qt::Key_Backspace);
  QAction *cut = new QAction ("Cut",this);
  cut->setShortcuts(QKeySequence::Cut);
  QAction *copy = new QAction ("Copy",this);
  copy->setShortcuts(QKeySequence::Copy);
  QAction *paste = new QAction ("Paste",this);
  paste->setShortcuts(QKeySequence::Paste);
  QAction *selectAll = new QAction ("Select all",this);
  selectAll->setShortcuts(QKeySequence::SelectAll);
  QAction *clearScr = new QAction ("Clear screen",this);
  clearScr->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_D);
  QAction *fullScreen = new QAction ("Full screen",this);
  fullScreen->setShortcut(Qt::Key_F11);
  fullScreen->setCheckable(true);

  menuFile->addAction(containersMng);
  connect(containersMng, &QAction::triggered, this,&AkiwakeMenuBar::openContainerManager);
  menuFile->addAction(sFile);
  connect(sFile, &QAction::triggered, this,&AkiwakeMenuBar::saveToFile);
  menuFile->addSeparator();
  menuFile->addAction("&Exit", qApp, SLOT(quit()),Qt::ALT + Qt::Key_F4);
  menuEdit->addAction(clearScr);
  connect(clearScr, &QAction::triggered, this,&AkiwakeMenuBar::clearScreen);
  menuEdit->addSeparator();
  menuEdit->addAction(del);
  connect(del, &QAction::triggered,line->textLine,&AkiwakeLineEdit::backspace);
  menuEdit->addAction(cut);
  connect(cut, &QAction::triggered,line->textLine,&AkiwakeLineEdit::cut);
  menuEdit->addAction(copy);
  connect(copy, &QAction::triggered,line->textLine,&AkiwakeLineEdit::copy);
  menuEdit->addAction(paste);
  connect(paste, &QAction::triggered,line->textLine,&AkiwakeLineEdit::paste);
  menuEdit->addSeparator();
  menuEdit->addAction(selectAll);
  connect(selectAll, &QAction::triggered,line->textLine,&AkiwakeLineEdit::selectAll);
  menuTools->addAction(fullScreen);
  connect(fullScreen, &QAction::triggered, this,&AkiwakeMenuBar::clearScreen);




}

void AkiwakeMenuBar::openContainerManager() {
  Containers *containersWindow = new Containers();
  containersWindow->exec();
  delete containersWindow;
}
void AkiwakeMenuBar::saveToFile() {

}
void AkiwakeMenuBar::clearScreen(){

}

void AkiwakeMenuBar::fScreen(){


}

