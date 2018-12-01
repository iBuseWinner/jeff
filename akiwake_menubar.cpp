#include "akiwake_menubar.h"
#include <QDebug>
#include <QApplication>

AkiwakeMenuBar::AkiwakeMenuBar(QWidget *parent) : QMenuBar (parent) {
QMenu *menuFile = this->addMenu("File");//Add File in Menu Bar
QAction *containersMng = new QAction("Container Manager",this);
containersMng->setShortcut(Qt::CTRL + Qt::Key_M); //Hotkey for Container Manager
menuFile->addAction(containersMng); //Add Container Manager in File
connect(containersMng, &QAction::triggered, this, &AkiwakeMenuBar::openContainerManager); //Event Container Manager on click
menuFile->addSeparator();//Add separator
menuFile->addAction("&Exit", qApp, SLOT(quit()), Qt::CTRL + Qt::Key_Q);//Add Exit in File and hotkey

}
void AkiwakeMenuBar::openContainerManager(){
    qDebug("openContainerManager");
}
