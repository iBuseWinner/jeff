cmake_minimum_required(VERSION 3.5)
project(cc-jeff)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Network Sql Widgets Xml REQUIRED)
find_package(Python COMPONENTS Development Interpreter REQUIRED)
add_compile_definitions(QT)

include_directories(
  .
  core
  core/database
  core/model
  core/model/nlp
  core/model/python
  dialogues
  dialogues/sources
  widgets
)

set (SRCS
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
  core/python-module.cpp
  core/python-module.h
  core/scenery.cpp
  core/scenery.h
  core/standard-templates.cpp
  core/standard-templates.h
  core/database/json.cpp
  core/database/json.h
  core/database/sqlite.cpp
  core/database/sqlite.h
  core/database/xml.h
  core/model/expression.h
  core/model/keystore.h
  core/model/message.h
  core/model/source.h
  core/model/aiml/aiml-parser.h
  core/model/nlp/cache.h
  core/model/nlp/cacher.cpp
  core/model/nlp/cacher.h
  core/model/nlp/options.h
  core/model/nlp/stringssearch.cpp
  core/model/nlp/stringssearch.h
  core/model/python/modulesdata.h
  core/model/python/object.h
  core/model/python/script.h
  dialogues/about.cpp
  dialogues/about.h
  dialogues/expression-editor.cpp
  dialogues/expression-editor.h
  dialogues/firststart.cpp
  dialogues/firststart.h
  dialogues/help.cpp
  dialogues/help.h
  dialogues/modal-handler.h
  dialogues/settings.cpp
  dialogues/settings.h
  dialogues/sources.cpp
  dialogues/sources.h
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
  widgets/spoiler.cpp
  widgets/spoiler.h
  widgets/styling.cpp
  widgets/styling.h
  jeff.cpp
  jeff.h
  main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS} resources.qrc)

target_link_libraries(
  ${CMAKE_PROJECT_NAME} PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Sql
    Qt5::Widgets
    Qt5::Xml
)
