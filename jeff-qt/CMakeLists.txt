cmake_minimum_required(VERSION 3.5)
project(jeff-cli)

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
  core-kit
  core-kit/database
  core-kit/model
  core-kit/model/nlp
  core-kit/model/python
  dialogues
  dialogues/phrase-editor
  dialogues/sources
  widgets
)

set(SRCS
  core-kit/basis.cpp
  core-kit/basis.h
  core-kit/core.cpp
  core-kit/core.h
  core-kit/history-processor.cpp
  core-kit/history-processor.h
  core-kit/jeff-core-kit.cpp
  core-kit/jeff-core-kit.h
  core-kit/local-server.cpp
  core-kit/local-server.h
  core-kit/notify-client.cpp
  core-kit/notify-client.h
  core-kit/extensions-manager.cpp
  core-kit/extensions-manager.h
  core-kit/standard-templates.cpp
  core-kit/standard-templates.h
  core-kit/database/json.cpp
  core-kit/database/json.h
  core-kit/database/sqlite.cpp
  core-kit/database/sqlite.h
  core-kit/extensions/daemon-process.cpp
  core-kit/extensions/daemon-process.h
  core-kit/extensions/extension.h
  core-kit/extensions/script.h
  core-kit/model/keystore.h
  core-kit/model/message.h
  core-kit/model/phrase.h
  core-kit/model/source.h
  core-kit/model/nlp/cache.h
  core-kit/model/nlp/cacher.cpp
  core-kit/model/nlp/cacher.h
  core-kit/model/nlp/expression.h
  core-kit/model/nlp/options.h
  core-kit/model/nlp/stringssearch.cpp
  core-kit/model/nlp/stringssearch.h
  core-kit/model/python/object.h
  core-kit/model/python/worker.cpp
  core-kit/model/python/worker.h
  main.cpp
)

add_executable(jeff-cli ${SRCS} jeff.cpp jeff.h resources.qrc)

target_link_libraries(
  jeff-cli PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Network
    Qt5::Sql
    ncursesw
)

set(SRCS_UI
  dialogues/about.cpp
  dialogues/about.h
  dialogues/add-script.cpp
  dialogues/add-script.h
  dialogues/extensions-viewer.cpp
  dialogues/extensions-viewer.h
  dialogues/firststart.cpp
  dialogues/firststart.h
  dialogues/help.cpp
  dialogues/help.h
  dialogues/modal-handler.h
  dialogues/phrase-editor.cpp
  dialogues/phrase-editor.h
  dialogues/settings.cpp
  dialogues/settings.h
  dialogues/sources-editor.cpp
  dialogues/sources-editor.h
  dialogues/extensions-viewer/extension-overview.cpp
  dialogues/extensions-viewer/extension-overview.h
  dialogues/phrase-editor/brief.cpp
  dialogues/phrase-editor/brief.h
  dialogues/phrase-editor/overview.cpp
  dialogues/phrase-editor/overview.h
  dialogues/phrase-editor/selector.cpp
  dialogues/phrase-editor/selector.h
  dialogues/sources-editor/create-source.cpp
  dialogues/sources-editor/create-source.h
  widgets/board.cpp
  widgets/board.h
  widgets/button.cpp
  widgets/button.h
  widgets/combobox.cpp
  widgets/combobox.h
  widgets/display.cpp
  widgets/display.h
  widgets/editlist.cpp
  widgets/editlist.h
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
  widgets/scripteditor.cpp
  widgets/scripteditor.h
  widgets/scrollfreezerwidget.h
  widgets/spoiler.cpp
  widgets/spoiler.h
  widgets/styling.cpp
  widgets/styling.h
  jeff-ui.cpp
  jeff-ui.h
)

add_executable(jeff-qt ${SRCS} ${SRCS_UI} resources.qrc ui-resources.qrc)

target_compile_definitions(jeff-qt PRIVATE JEFF_WITH_QT_WIDGETS)

target_link_libraries(
  jeff-qt PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Sql
    Qt5::Widgets
)
