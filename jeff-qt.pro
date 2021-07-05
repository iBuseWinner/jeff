QT += core gui widgets sql

TARGET = jeff-qt
TEMPLATE = app

INCLUDEPATH += /usr/include/python3.9

LIBS += -lpython3.9

DEFINES += QT_DEPRECATED_WARNINGS \
    # ADISPLAY_DEBUG \
    # ADISPLAY_ADDMSG_DEBUG \
    # SQLITE_DEBUG \
    # SQLITE_SCANSOURCE_DEBUG \
    # ADISPLAY_START_DEBUG \

CONFIG += c++2a

SOURCES += \
    core/basis.cpp \
    core/core.cpp \
    core/database/json.cpp \
    core/history-processor.cpp \
    core/python-module.cpp \
    core/scenery.cpp \
    core/standard-templates.cpp \
    dialogues/sources/create-source.cpp \
    dialogues/settings.cpp \
    dialogues/sources.cpp \
    dialogues/sources/edit-source.cpp \
    main.cpp \
    jeff.cpp \
    core/database/sqlite.cpp \
    dialogues/help.cpp \
    dialogues/firststart.cpp \
    dialogues/about.cpp \
    widgets/a_button.cpp \
    widgets/a_expressionlist.cpp \
    widgets/a_message.cpp \
    widgets/a_board.cpp \
    widgets/a_display.cpp \
    widgets/a_line.cpp \
    widgets/a_lineedit.cpp \
    widgets/a_menubar.cpp \
    core/nlp-module.cpp \
    widgets/a_explanationlabel.cpp \
    widgets/a_sourcelist.cpp

HEADERS += \
    jeff.h \
    core/basis.h \
    core/core.h \
    core/database/json.h \
    core/model/expression.h \
    core/history-processor.h \
    core/model/message.h \
    core/model/nlp/cache.h \
    core/model/nlp/linked-cache.h \
    core/model/nlp/options.h \
    core/model/nlp/responsewo.h \
    core/python-module.h \
    core/model/source.h \
    core/scenery.h \
    core/database/sqlite.h \
    core/standard-templates.h \
    dialogues/modal-handler.h \
    dialogues/sources/create-source.h \
    dialogues/help.h \
    dialogues/firststart.h \
    dialogues/about.h \
    dialogues/settings.h \
    dialogues/sources.h \
    dialogues/sources/edit-source.h \
    widgets/a_button.h \
    widgets/a_expressionlist.h \
    widgets/a_message.h \
    widgets/a_board.h \
    widgets/a_display.h \
    widgets/a_line.h \
    widgets/a_lineedit.h \
    widgets/a_menubar.h \
    core/nlp-module.h \
    widgets/a_explanationlabel.h \
    widgets/a_sourcelist.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += l10n/jeff_ru.ts
