cmake_minimum_required(VERSION 3.5)
project(cclc-jeff)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Sql Xml REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

include_directories(
    .
    core
    core/database
    model
    model/nlp
    dialogues
    dialogues/sources
    widgets
)

set (SRCS
    core/basis.cpp
    core/core.cpp
    core/database/json.cpp
    core/history-processor.cpp
    core/python-module.cpp
    core/scenery.cpp
    core/standard-templates.cpp
    dialogues/sources/create-source.cpp
    dialogues/settings.cpp
    dialogues/sources.cpp
    dialogues/sources/edit-source.cpp
    main.cpp
    jeff.cpp
    core/database/sqlite.cpp
    dialogues/help.cpp
    dialogues/firststart.cpp
    dialogues/about.cpp
    widgets/button.cpp
    widgets/expressionlist.cpp
    widgets/message.cpp
    widgets/board.cpp
    widgets/display.cpp
    widgets/line.cpp
    widgets/lineedit.cpp
    widgets/menubar.cpp
    core/nlp-module.cpp
    widgets/explanationlabel.cpp
    widgets/sourcelist.cpp
    jeff.h
    core/basis.h
    core/core.h
    core/database/json.h
    core/model/expression.h
    core/history-processor.h
    core/model/message.h
    core/model/nlp/cache.h
    core/model/nlp/options.h
    core/model/nlp/responsewo.h
    core/model/nlp/stringssearch.h
    core/model/nlp/stringssearch.cpp
    core/python-module.h
    core/model/source.h
    core/scenery.h
    core/database/sqlite.h
    core/standard-templates.h
    dialogues/modal-handler.h
    dialogues/sources/create-source.h
    dialogues/help.h
    dialogues/firststart.h
    dialogues/about.h
    dialogues/settings.h
    dialogues/sources.h
    dialogues/sources/edit-source.h
    widgets/button.h
    widgets/expressionlist.h
    widgets/message.h
    widgets/board.h
    widgets/display.h
    widgets/line.h
    widgets/lineedit.h
    widgets/menubar.h
    core/nlp-module.h
    widgets/explanationlabel.h
    widgets/sourcelist.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS} resources.qrc)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml Qt5::Sql Python::Python)
