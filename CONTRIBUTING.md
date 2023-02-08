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

> В классе SQLite содержатся методы для высокоуровневой работы с базами данных Джеффа. Например, именно SQLite содержит методы добавления выражений, создания новых источников, загрузки всех источников из базы данных и поиска реагентов по активаторам в пользовательском вводе.

* core-kit/extensions/daemon-process.hpp & daemon-process.cpp

> Класс DaemonProcess содержит обёртку для внешних процессов, которые используются при запуске расширений Джеффа.

## Описание процессов, которые происходят при различных событиях

1. Запускается main.cpp/main(), устанавливается уровень логгирования, встраивается транслятор и запускается окно/TUI.
2. Создаются основные классы внутри класса jeff-ui.hpp/Jeff(): Core |-> Basis, HProcessor, Server, NotifyClient и ExtensionsManager.
3. Basis |-> Json, SQLite, Cacher
4. В Json создаётся папка настроек, если не существует; формируется файл логгирования.
5. В SQLite создаётся соединение с БД, которое используется в редакторах выражений и источников.
6. После этого загружаются источники и значения памяти.
7. Следом происходит инициализация сервера и NotifyClient.
8. Инициализируется ExtensionsManager и подгружает список расширений.
