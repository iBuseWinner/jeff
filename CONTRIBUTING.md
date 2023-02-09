# Jeff development

## Code guidelines

1. Indent size - 2 spaces.
2. Each declaration of a class, enumeration, namespace, function, method and type (except for lists of class instances) must be accompanied by a capacious comment (see below).
3. If possible, it is better to fit the construction in one line or without curly braces. The opening curly brace should ideally be on the same line as the declaration or construct.
4. Variables should have understandable names, otherwise the variables should only help in solving the problem and disappear instantly.
5. It's better to write `TBD` in a comment than to never write a comment.
6. If there are many similar designs, it is better to align them.

### Comments in Jeff Qt

Single-line comments with two slashes `//` are allowed, all other comments are done in Doxygen style.

Functions and methods are commented like this:

```c++
/*! @brief Comment text. */

/*! @brief One more comment text.
 * @details You can reveal the details of the work and / or purpose. */
```

Class constructors and destructors are almost always commented without a description:

```c++
/*! @brief The constructor/destructor. */
```

If it is necessary to show the diagram of the dialog box, it is displayed approximately like this:

```c++
/*! @brief The constructor.
 *  @details Layout scheme:
 *  <-------------->
 *  [    Title     ]
 *  [  Start text  ]
 *  [ Bottom  line ]
 *  {<->}{Close btn}
 *  <-------------->  */
```

If a class method is described in a `.cpp` file, this must be indicated in a comment:

```c++
// Functions described in `extensions-viewer.cpp`:
```

## Jeff Qt

**Jeff Qt** consists of its own set of widgets as well as a set of Core tools. The following are the main classes and data structures used in Jeff:

* core-kit/database/json.hpp & json.cpp

The **Json** class contains methods for saving and restoring message history, expression cache, source lists, extensions, and memory values.

* core-kit/database/sqlite.hpp & sqlite.cpp

> The **Check** enum describes the options for checking a database. Databases are checked each time before opening.

> The **ToDo** enum describes an action to be performed on the database. Each enumeration option corresponds to a SQL query, described in `SQLite::exec`.

> The **SQLite** class contains methods for working with Jeff's high-level databases. For example, it is SQLite that contains methods for adding expressions, creating new sources, loading all sources from the database, and searching for reagents by activators in user input.

* core-kit/extensions/daemon-process.hpp & daemon-process.cpp

> The **DaemonProcess** class contains a wrapper for external processes that are used when running Jeff's extensions.

* core-kit/extensions/extension.hpp & extension.cpp

> The **ExtensionMeta** class represents one of Jeff's extensions and provides methods for storing and reading metadata about them.

* core-kit/extensions/script.hpp & script.cpp

> The **ScriptType** enum specifies the types of scripts that are used in Jeff.

> The **ScriptMeta** class represents one of Jeff's scripts and provides methods for storing and reading metadata about them.

* core-kit/model/nlp/cache.hpp

> The **Cache** type is just a list of expressions. And the **CoverageCache** type is a list of expressions and its coverage metadata.

* core-kit/model/nlp/cacher.hpp & cacher.cpp

> The **Cacher** class manages the expression cache. In other words, Cacher keeps Jeff's master expression cache.

* core-kit/model/nlp/expression.hpp

> The **Expression** class contains information about a single expression of JCK module.

> The **ExpressionCoverage** struct contains Expression, coverage indices dict and total percent of coverage of input by expression activators.

* core-kit/model/nlp/options.hpp

> The **Options** type is a str-str dict for phrase options.

* core-kit/model/nlp/stringssearch.hpp & stringssearch.cpp

> The **StringSearch** namespace contains several functions, **Intersects** enum and **WordMetadata** struct needed for searching activators in user input.

* core-kit/model/python/object.hpp

> The **Object** class provides a wrapper for proper memory management of a Python object. Used only by `PythonWorker` class.

* core-kit/model/python/worker.hpp & worker.cpp

> The **PythonWorker** class contains the logic of working with Python scripts.

* core-kit/model/keystore.hpp

> The **KeyStore** type is a str-json dict for storing some values used by extensions and scripts.

* core-kit/model/message.hpp

> The **MessageMeta** class is a representation of a single message in Jeff.

> The **Author**, **ContentType** and **Theme** enums used to describe message properties.

* core-kit/model/phrase.hpp

> The **Phrase** class represents a sentence in SQL database.

* core-kit/model/source.hpp

> The **Source** class contains data about a source of phrases for the JCK.

* core-kit/basis.hpp & basis.cpp

> The **Basis** class provides methods for intra-component work. In details, Basis does a lot of important things:  
>  - provides identifiers for saving settings  
>  - reads and writes a list of sources, settings and message history  
>  - stores a list of sources and cached expressions  
>  - checks the settings file for errors  
>  - stores memory map.

* core-kit/core.hpp & core.cpp

> The **Core** class manages objects associated with receiving messages from different sources (as kbd, extensions, etc.).

* core-kit/extensions-manager.hpp & extensions-manager.cpp

> The **ExtensionsManager** class contains methods of working with extensions.

* core-kit/history-processor.hpp & history-processor.cpp

> The **HProcessor** class stores, exports and loads message history.

* core-kit/jeff-core-kit.hpp & jeff-core-kit.cpp

> The **JCK** class looks for phrases in user input and answers to them.

> The **JCKController** class is responsible for moving `JCK` to a separate thread.

* core-kit/local-server.hpp & local-server.cpp

> The **Server** class manages incoming connections and receives messages.

* core-kit/notify-client.hpp & notify-client.cpp

> The **NotifyClient** class sends new messages over the network.

* core-kit/standard-templates.hpp & standard-templates.cpp

> The **StandardTemplates** class provides some kinds of widgets and operations by command in user input.

* dialogues/*

> Dialogues folder contains special widgets that Jeff shows to users when they want to change some settings. In general, they're little windows inside Jeff Display.

* l10n/*

> This folder contains some localizations - in `*.ts` format and compiled `*.qm` format.

* lib/*

> This folder contains Jeff dependencies such as `maddy`, Markdown interpreter, and `Yelloger`, simple single-file logger.

* widgets/*

> This folder contains widgets used in Jeff. These widgets are customed and have two themes by default (light and dark). And the `widgets/layouts/*` folder contains three layouts for them with reactive paradigm of constructing.

* jeff-ui.hpp & jeff-ui.cpp

> The **Jeff** class is a main Jeff window, with `Display` and input `Line` main widgets.

* jeff.hpp & jeff.cpp

> The **Jeff** class described in these files used to build TUI-version of Jeff.

## Description of the main processes in Jeff

### Jeff Startup

1. Main.cpp/main() is run, the logging level is set, the translator is built in, and the window/TUI is launched.
2. Main classes are created inside jeff-ui.hpp/Jeff() class: `Core |-> Basis, HProcessor, Server, NotifyClient, ExtensionsManager`.
3. `Basis |-> Json, SQLite, Cacher`
4. A settings folder is created in Json if it does not exist; log file is generated.
5. A database connection is created in SQLite, which is used in the editors of expressions and sources.
6. After that, the sources and values of the memory are loaded.
7. Next, the server and NotifyClient are initialized.
8. The ExtensionsManager is initialized and loads the list of extensions.
9. JCK is initialized in a separate thread, a connection to the database for JCK is created, PythonWorker is initialized and the expression cache is read. A random number generator is created.
10. The StandardTemplates class is initialized.
11. The settings file and default source are checked, and the monologue mode and server port variables are set.
12. Jeff's server starts.
13. The presence of settings is checked, if they are - they are applied, if they are not there - the default settings are recorded.
14. Extensions are launched.
15. If history storage is enabled, it is loaded, then displayed in the Display.
16. Custom scanner and composer are loaded, if any.
17. If the greeting message is enabled, it starts.

### User sent message

1. The message is sent to `Core`.
2. The message is added to the story and displayed on the screen.
3. `StandardTemplates` looks for commands in the message; if anyone found, `StandardTemplates` performs certain actions, and the message does not go further.
4. Extensions, which of these always accept input, are notified of a new message from the user. If a scenario is running, only that scenario is notified and the message does not go any further.
5. The message is sent to `JCK`.
6. If there is a custom scanner, the expression is sent to it. If the scanner returns a message, it is sent to the `Core` and `JCK` is terminated. If the scanner returned a selection of expressions, they are unpacked and the search continues. If the scanner returns nothing, `JCK` sends an `empty` signal and exits.
7. If there is no scanner, the expression is searched in the cache. But if the selection is empty, or the user's previous message is identical to the current one, and the `seacrhInDbWhenRepeated` setting is enabled, the expression is searched across all sources in `SQLite`.
8. If the selection is empty, `JCK` sends an `empty` signal and exits.
9. The best candidates are selected. The selection mechanism is described in [README.md](README.md#select_candidates).
10. If there is a custom composer, the expression is sent to it. If the scanner returns a message, it is sent to the `Core`, otherwise an `empty` signal is sent. `JCK` exits.
11. If there is no composer, the built-in composer is executed, which connects the candidates in one message. But if the response-to-input ratio is less than 1/3, and the lookup was performed only in the cache, it is performed in the database.
12. If the result is not empty, it is sent to the `Core`, if empty, the `empty` signal is sent. `JCK` exits.
13. If `Core` received a response from `JCK`, then after the delay time specified in the settings, the response is added to the history, sent to extensions and displayed on the screen.
14. If `Core` received an `empty` signal, those extensions that did not receive a message before `JCK` (see 4th) are notified.
15. If `Core` received one of the answer options from the extensions, it is also added to the history after the delay time specified in the settings, sent to the extensions (again!) and displayed on the screen.
