# Jeff development

## Code guidelines

*TBD*

## Jeff Qt

**Jeff Qt** consists of its own set of widgets as well as a set of Core tools. The following are the main classes and data structures used in Jeff:

### core-kit/database/json.hpp & json.cpp

The **Json** class contains methods for saving and restoring message history, expression cache, source lists, extensions, and memory values.

### core-kit/database/sqlite.hpp & sqlite.cpp

The **Check** enum describes the options for checking a database. Databases are checked each time before opening.

The **ToDo** enum describes an action to be performed on the database. Each enumeration option corresponds to a SQL query, described in `SQLite::exec`.

