cmake_minimum_required(VERSION 3.5)
project(jeff)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Network Sql Widgets REQUIRED)
find_package(Python COMPONENTS Development Interpreter REQUIRED)

include_directories(
  .
  core
  core/database
  core/model
  core/model/nlp
  core/model/python
  dialogues
  dialogues/phrase-editor
  dialogues/sources
  widgets
)

set(SRCS
  core/basis.cpp
  core/basis.h
  core/core.cpp
  core/core.h
  core/history-processor.cpp
  core/history-processor.h
  core/local-server.cpp
  core/local-server.h
  core/nlp-module.cpp
  core/nlp-module.h
  core/notify-client.cpp
  core/notify-client.h
  core/python-module.cpp
  core/python-module.h
  core/standard-templates.cpp
  core/standard-templates.h
  core/database/json.cpp
  core/database/json.h
  core/database/sqlite.cpp
  core/database/sqlite.h
  core/model/expression.h
  core/model/keystore.h
  core/model/message.h
  core/model/phrase.h
  core/model/source.h
  core/model/nlp/cache.h
  core/model/nlp/cacher.cpp
  core/model/nlp/cacher.h
  core/model/nlp/options.h
  core/model/nlp/stringssearch.cpp
  core/model/nlp/stringssearch.h
  core/model/python/object.h
  core/model/python/script.h
  main.cpp
)

add_executable(jeff ${SRCS} jeff.cpp jeff.h resources.qrc)

target_link_libraries(
  jeff PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Network
    Qt5::Sql
    ncursesw
)

set(SRCS_UI
  dialogues/about.cpp
  dialogues/about.h
  dialogues/firststart.cpp
  dialogues/firststart.h
  dialogues/help.cpp
  dialogues/help.h
  dialogues/modal-handler.h
  dialogues/phrase-editor.cpp
  dialogues/phrase-editor.h
  dialogues/settings.cpp
  dialogues/settings.h
  dialogues/sources.cpp
  dialogues/sources.h
  dialogues/phrase-editor/brief.cpp
  dialogues/phrase-editor/brief.h
  dialogues/phrase-editor/overview.cpp
  dialogues/phrase-editor/overview.h
  dialogues/phrase-editor/selector.cpp
  dialogues/phrase-editor/selector.h
  dialogues/sources/create-source.cpp
  dialogues/sources/create-source.h
  widgets/board.cpp
  widgets/board.h
  widgets/button.cpp
  widgets/button.h
  widgets/combobox.cpp
  widgets/combobox.h
  widgets/display.cpp
  widgets/display.h
  widgets/explanationlabel.cpp
  widgets/explanationlabel.h
  widgets/line.cpp
  widgets/line.h
  widgets/lineedit.cpp
  widgets/lineedit.h
  widgets/list.cpp
  widgets/list.h
  widgets/menu.cpp
  widgets/menu.h
  widgets/menubar.cpp
  widgets/menubar.h
  widgets/message.cpp
  widgets/message.h
  widgets/pairwidget.cpp
  widgets/pairwidget.h
  widgets/scrollfreezerwidget.h
  widgets/spoiler.cpp
  widgets/spoiler.h
  widgets/styling.cpp
  widgets/styling.h
  jeff-ui.cpp
  jeff-ui.h
)

add_executable(jeff-ui ${SRCS} ${SRCS_UI} resources.qrc ui-resources.qrc)

target_compile_definitions(jeff-ui PRIVATE JEFF_WITH_QT_WIDGETS)

target_link_libraries(
  jeff-ui PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Sql
    Qt5::Widgets
)
