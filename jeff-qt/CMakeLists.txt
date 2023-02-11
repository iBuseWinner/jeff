cmake_minimum_required(VERSION 3.5)
project(jeff-cli)

# Common part

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Network Sql Widgets REQUIRED)
find_package(Python COMPONENTS Development Interpreter REQUIRED)

add_compile_options(-flto -Wall -Wextra -pedantic -Wstrict-aliasing -Wno-format -Wno-format-security)

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
  lib/Yelloger/include
  widgets
)

# Jeff-Cli part

set(SRCS
  core-kit/basis.cpp
  core-kit/basis.hpp
  core-kit/core.cpp
  core-kit/core.hpp
  core-kit/history-processor.cpp
  core-kit/history-processor.hpp
  core-kit/jeff-core-kit.cpp
  core-kit/jeff-core-kit.hpp
  core-kit/local-server.cpp
  core-kit/local-server.hpp
  core-kit/notify-client.cpp
  core-kit/notify-client.hpp
  core-kit/extensions-manager.cpp
  core-kit/extensions-manager.hpp
  core-kit/standard-templates.cpp
  core-kit/standard-templates.hpp
  core-kit/database/json.cpp
  core-kit/database/json.hpp
  core-kit/database/sqlite.cpp
  core-kit/database/sqlite.hpp
  core-kit/extensions/daemon-process.cpp
  core-kit/extensions/daemon-process.hpp
  core-kit/extensions/extension.cpp
  core-kit/extensions/extension.hpp
  core-kit/extensions/script.cpp
  core-kit/extensions/script.hpp
  core-kit/model/keystore.hpp
  core-kit/model/message.hpp
  core-kit/model/phrase.hpp
  core-kit/model/source.hpp
  core-kit/model/nlp/cache.hpp
  core-kit/model/nlp/cacher.cpp
  core-kit/model/nlp/cacher.hpp
  core-kit/model/nlp/expression.hpp
  core-kit/model/nlp/options.hpp
  core-kit/model/nlp/stringssearch.cpp
  core-kit/model/nlp/stringssearch.hpp
  core-kit/model/python/object.hpp
  core-kit/model/python/worker.cpp
  core-kit/model/python/worker.hpp
  main.cpp
)

add_executable(jeff-cli ${SRCS} jeff.cpp jeff.hpp resources.qrc)

# Available definitions:
#
# YELLOGER_TRACING
# target_compile_definitions(jeff-cli PRIVATE YELLOGER_TRACING)

target_link_libraries(
  jeff-cli PRIVATE
    Python::Python
    Qt5::Core
    Qt5::Network
    Qt5::Sql
    ncursesw
)

# Jeff-Qt part

set(HOEDOWN_LIB_SRC
  lib/hoedown/src/autolink.c
  lib/hoedown/src/autolink.h
  lib/hoedown/src/buffer.c
  lib/hoedown/src/buffer.h
  lib/hoedown/src/document.c
  lib/hoedown/src/document.h
  lib/hoedown/src/escape.c
  lib/hoedown/src/escape.h
  lib/hoedown/src/html_blocks.c
  lib/hoedown/src/html_smartypants.c
  lib/hoedown/src/html.c
  lib/hoedown/src/html.h
  lib/hoedown/src/stack.c
  lib/hoedown/src/stack.h
  lib/hoedown/src/version.c
  lib/hoedown/src/version.h
)

set(SRCS_UI
  ${HOEDOWN_LIB_SRC}
  dialogues/about.cpp
  dialogues/about.hpp
  dialogues/add-script.cpp
  dialogues/add-script.hpp
  dialogues/dialog.hpp
  dialogues/extensions-viewer.cpp
  dialogues/extensions-viewer.hpp
  dialogues/firststart.cpp
  dialogues/firststart.hpp
  dialogues/help.cpp
  dialogues/help.hpp
  dialogues/modal-handler.hpp
  dialogues/phrase-editor.cpp
  dialogues/phrase-editor.hpp
  dialogues/settings.cpp
  dialogues/settings.hpp
  dialogues/sources-editor.cpp
  dialogues/sources-editor.hpp
  dialogues/extensions-viewer/brief.cpp
  dialogues/extensions-viewer/brief.hpp
  dialogues/extensions-viewer/card.cpp
  dialogues/extensions-viewer/card.hpp
  dialogues/extensions-viewer/overview.cpp
  dialogues/extensions-viewer/overview.hpp
  dialogues/phrase-editor/brief.cpp
  dialogues/phrase-editor/brief.hpp
  dialogues/phrase-editor/overview.cpp
  dialogues/phrase-editor/overview.hpp
  dialogues/phrase-editor/selector.cpp
  dialogues/phrase-editor/selector.hpp
  dialogues/sources-editor/create-source.cpp
  dialogues/sources-editor/create-source.hpp
  widgets/board.cpp
  widgets/board.hpp
  widgets/button.cpp
  widgets/button.hpp
  widgets/combobox.cpp
  widgets/combobox.hpp
  widgets/display.cpp
  widgets/display.hpp
  widgets/editlist.cpp
  widgets/editlist.hpp
  widgets/explanationlabel.cpp
  widgets/explanationlabel.hpp
  widgets/line.cpp
  widgets/line.hpp
  widgets/lineedit.cpp
  widgets/lineedit.hpp
  widgets/list.cpp
  widgets/list.hpp
  widgets/menu.cpp
  widgets/menu.hpp
  widgets/menubar.cpp
  widgets/menubar.hpp
  widgets/message.cpp
  widgets/message.hpp
  widgets/pairwidget.cpp
  widgets/pairwidget.hpp
  widgets/scripteditor.cpp
  widgets/scripteditor.hpp
  widgets/scrollarea.hpp
  widgets/scrollfreezerwidget.hpp
  widgets/spoiler.cpp
  widgets/spoiler.hpp
  widgets/styling.cpp
  widgets/styling.hpp
  widgets/textedit.cpp
  widgets/textedit.hpp
  widgets/layouts/grid.hpp
  widgets/layouts/linears.hpp
  jeff-ui.cpp
  jeff-ui.hpp
)

add_executable(jeff-qt ${SRCS} ${SRCS_UI} resources.qrc ui-resources.qrc)

# Available definitions:
#
# YELLOGER_TRACING
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
