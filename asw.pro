QT       += core gui widgets sql

TARGET = asw
TEMPLATE = app

INCLUDEPATH += /usr/include/python3.8

LIBS += -lpython3.8

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    core/basis.cpp \
    core/core.cpp \
    core/history-processor.cpp \
    core/python-handler.cpp \
    core/standard-templates.cpp \
    dialogues/sources/create-source.cpp \
    dialogues/settings.cpp \
    dialogues/sources.cpp \
    main.cpp \
    asw.cpp \
    core/sqlite.cpp \
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
    core/basis.h \
    core/core.h \
    core/history-processor.h \
    core/message.h \
    core/python-handler.h \
    core/source.h \
    core/sqlite.h \
    core/standard-templates.h \
    dialogues/sources/create-source.h \
    dialogues/help.h \
    dialogues/firststart.h \
    dialogues/about.h \
    dialogues/settings.h \
    dialogues/sources.h \
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

RESOURCES += \
    resources.qrc

TRANSLATIONS += l10n/asw_ru.ts
