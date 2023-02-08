# Jeff development

## Code guidelines

*TBD*

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
