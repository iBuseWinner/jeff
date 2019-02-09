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
    widgets/akiwake_pushbutton.cpp \
    widgets/akiwake_message.cpp \
    core/sqlite.cpp \
    widgets/akiwake_board.cpp \
    widgets/akiwake_line.cpp \
    widgets/akiwake_display.cpp \
    core/thinking.cpp \
    core/settingsstore.cpp \
    dialogues/containers.cpp \
    dialogues/akiwake_dialog.cpp \
    widgets/akiwake_containerslist.cpp \
    dialogues/createcontainer.cpp \
    widgets/akiwake_lineedit.cpp \
    widgets/akiwake_menubar.cpp \
    core/history_processor.cpp \
    dialogues/help.cpp \
    dialogues/firststart.cpp \
    dialogues/about.cpp

HEADERS += \
    asw.h \
    core/handlers.h \
    widgets/akiwake_pushbutton.h \
    widgets/akiwake_message.h \
    core/sqlite.h \
    widgets/akiwake_board.h \
    widgets/akiwake_line.h \
    widgets/akiwake_display.h \
    core/thinking.h \
    core/settingsstore.h \
    dialogues/containers.h \
    dialogues/akiwake_dialog.h \
    widgets/akiwake_containerslist.h \
    core/containersstruct.h \
    dialogues/createcontainer.h \
    widgets/akiwake_lineedit.h \
    widgets/akiwake_menubar.h \
    core/history_processor.h \
    dialogues/help.h \
    dialogues/firststart.h \
    dialogues/about.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
