QT       += core gui widgets sql

TARGET = asw
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    asw.cpp \
    core/handlers.cpp \
    core/sqlite.cpp \
    core/thinking.cpp \
    core/settingsstore.cpp \
    dialogues/containers.cpp \
    dialogues/createcontainer.cpp \
    core/history_processor.cpp \
    dialogues/help.cpp \
    dialogues/firststart.cpp \
    dialogues/about.cpp \
    core/standardtemplates.cpp \
    widgets/a_message.cpp \
    widgets/a_board.cpp \
    widgets/a_containerslist.cpp \
    widgets/a_display.cpp \
    widgets/a_line.cpp \
    widgets/a_lineedit.cpp \
    widgets/a_menubar.cpp \
    widgets/a_pushbutton.cpp

HEADERS += \
    asw.h \
    core/handlers.h \
    core/sqlite.h \
    core/thinking.h \
    core/settingsstore.h \
    dialogues/containers.h \
    core/containersstruct.h \
    dialogues/createcontainer.h \
    core/history_processor.h \
    dialogues/help.h \
    dialogues/firststart.h \
    dialogues/about.h \
    core/standardtemplates.h \
    widgets/a_message.h \
    widgets/a_board.h \
    widgets/a_containerslist.h \
    widgets/a_display.h \
    widgets/a_line.h \
    widgets/a_lineedit.h \
    widgets/a_menubar.h \
    widgets/a_pushbutton.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
