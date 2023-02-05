# jeff

Jeff is a highly customizable, extensible and cross-platform chatbot with a MIT license.

## Capabilities

1. Interaction in the "question-answer" mode.
2. Interaction in scenarios.
3. Interaction through extensions communicating with Jeff over TCP sockets.
4. [In development] Multi-user interaction via HTTP server.
5. [In development] Interaction via Telegram Bot.

## Features

1. TCP-server with JSON message format.
2. Extensions Notifier.
3. SQLite databases for phrases.
4. Storage of message history.
5. Extensions and scripts configurations with additional Python API - `jeff-api`.
6. [In development] Bundles with pre-installed extensions.

## Project structure

There are several subprojects:

1. `jeff-qt` desktop app
2. `jeff-api` extensions API for Python
3. `jeff-core` server

## Screenshots

![Alice AIML engine inside Jeff](arts/shots/shot1.png)
![GPT-3 inside Jeff](arts/shots/shot2.png)

## Databases development and Jeff Core Kit

### `*.j.db` format

Jeff uses the SQLite portable database format. Each individual database must have a `sources` table. If you want to create it manually, use the query: `CREATE TABLE "sources" ("source" text NOT NULL UNIQUE, "title" text, "weight" integer)`. `Source` field means table name in this DB, `title` is displayed in Jeff and `weight` is an optional field that prioritizes this expression source over others, which affects the layout of the response.

Each individual source is listed in the `sources` table and created with the query: `CREATE TABLE "table_name" ("address" integer NOT NULL UNIQUE, "expression" text, "links" text, "exec" integer NOT NULL, "adprops" TEXT, PRIMARY KEY("address" AUTOINCREMENT))`.

### Properties `weight` and `adprops` in action

*Example 1.* Let the `t1` table contain an expression that looks for the `Turn on the TV` activator and issue the `Good!` reagent on it, while the `t2` table contains an expression that launches the script for turning on the TV on exactly the same activator. *There are two options for improving the output of the second expression:* you can set the weight for the table (so that all expressions from this table acquire such a weight), or you can set the weight for a specific expression (the `adprops` field stores JSON, the weight is set as an integer).

*Example 2.* Let there be an expression in table `t1` that looks for the `Turn on the light in the bedroom` activator, while there is another expression that looks for the `Turn on the light in the kitchen` activator. If the user enters `Turn on the light` or even `Turn on the lights` twice, the first time a random light will turn on, and the second time another light will turn on. This will happen because the percentage of `total_POC` input covered by both the first and second activators will be equal, and other things being equal, a random expression is chosen. But then the expression that was used less often than the others is always selected. In this way, a uniform use of the cache is achieved. *If you want to set one expression as the preferred expression in case of incomplete activation,* set its weight one or more units higher.

*Example 3.* Let the table `t1` contain two expressions with the same activators, which, nevertheless, have different additional properties. *If you want to process them and compose the response yourself,* write your own implementation of the composer in Python and include it in Jeff. *And if you also want to scan sources yourself or even connect your own sources,* write your own implementation of the scanner in Python and connect it too.

### Complete set of conditions for the selection of candidates

Expressions compete with each other for the right to be a reagent for a given activator. There are the following conditions for choosing an expression (in descending order of importance):

1. Is the set of candidates empty?
2. Is the current candidate consonant (e.g. usable with other expressions)?
3. Are there any intersections with other expressions?
4. Is the coverage percentage greater than another expression?
5. And the weight of the expression is greater?
6. Has the expression been used in less cases?
7. Everything is up to chance.

## Usable settings

1. Monologue mode. Allows Jeff to communicate with himself. Setting: `core-kit/ismonologuemodeenabled` (bool)
2. Output delay. Sets the time period in milliseconds in which the response message will be displayed and can be used to simulate human delay. Settings: `core-kit/isdelayenabled` (bool), `core-kit/mindelay` (ms), `core-kit/maxdelay` (ms)
3. Save history. Can save all your correspondence with Jeff. Setting: `core-kit/ishistorykeepingenabled` (bool)
4. Greeting. Sets the first message that is automatically sent to Jeff on startup. Settings: `core-kit/isgreetingsenabled` (bool), `core-kit/greetings` (str)
5. Script exit message. Keyword/expression that forces the script to end. Setting: `core-kit/scenarioexitter`
6. Server port. The port on which Jeff will listen for incoming connections from extensions. Setting: `core-kit/serverport` (uint16)
7. The time Jeff waits after sending the `terminate` signal and before forcing the extension to terminate. Setting: `core-kit/extensionkillsec` (sec)
8. Force search in the sources, not in the cache, if the user's current input is identical to his previous one. Setting: `core-kit/repeatedexplicitindb` (bool)
9. Default source. Declares a table in the database to which phrases that Jeff learned on his own or with the help of extensions will be added. Settings: `sources/defaultsourcepath`, `sources/defaultsourcecontainer` (both str)
10. Authorization with a key on the server. Jeff will only process messages that contain the key in JSON (note that `jeff-qt` sockets do not use encryption). Settings: `server/authorize` (bool), `server/auth_key` (str)

**Note:** Depending on whether you are using Jeff Qt or jeff-core, the configuration files may be in different locations.

## Extensions and scripts development

### `*.j.json` scripts' format

*TBD*

### `extension*.j.json` format

*TBD*

### Using `jeff-api` in your projects

*TBD*

## Building and running

### `jeff-qt`

*TBD*

### `jeff-core`

*TBD*
