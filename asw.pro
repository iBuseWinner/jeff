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
    core/core-methods.cpp \
    core/core.cpp \
    core/history-processor.cpp \
    core/standard-templates.cpp \
    dialogues/containers/createcontainer.cpp \
    dialogues/settings.cpp \
    main.cpp \
    asw.cpp \
    core/sqlite.cpp \
    dialogues/containers.cpp \
    dialogues/help.cpp \
    dialogues/firststart.cpp \
    dialogues/about.cpp \
    widgets/a_button.cpp \
    widgets/a_message.cpp \
    widgets/a_board.cpp \
    widgets/a_containerslist.cpp \
    widgets/a_display.cpp \
    widgets/a_line.cpp \
    widgets/a_lineedit.cpp \
    widgets/a_menubar.cpp \
    core/nlp-module.cpp \
    widgets/settings/a_explanationlabel.cpp

HEADERS += \
    asw.h \
    core/container.h \
    core/core-methods.h \
    core/core.h \
    core/history-processor.h \
    core/message.h \
    core/sqlite.h \
    core/standard-templates.h \
    dialogues/containers.h \
    dialogues/containers/createcontainer.h \
    dialogues/help.h \
    dialogues/firststart.h \
    dialogues/about.h \
    dialogues/settings.h \
    widgets/a_button.h \
    widgets/a_message.h \
    widgets/a_board.h \
    widgets/a_containerslist.h \
    widgets/a_display.h \
    widgets/a_line.h \
    widgets/a_lineedit.h \
    widgets/a_menubar.h \
    core/nlp-module.h \
    widgets/settings/a_explanationlabel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

TRANSLATIONS += l10n/asw_ru.ts
