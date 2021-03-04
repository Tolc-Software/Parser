# Logging #

This file contains how this program uses logging, conventions etc.

Logging is done via [spdlog](https://github.com/gabime/spdlog). **It should not be configured from within the Parser library. The log level, the logging syntax etc. is up to the user of this library.**

## Where/what to log ##

External logging (using `spdlog::{warn,error}`) is done **exclusively** in the `visitor` functions (i.e. `VisitNamespaceDecl`, `VisitEnumDecl` etc.). This is to avoid cluttering. It also forces the developer to log at a certain level. Prefer logging `Failed to parse return type: {returnType}` rather than `Failed to parse third level allocator in vector<vector<vector<...>>>`.

Internal logging (using `spdlog::debug`) can be used on multiple levels.

## Log configuration ##

Should only be done once as close to the start of the program as possible. Preferably only one time with one function.

### Log level ###

Note that log level and is set by the user of this library.

#### Debug ####

Should mostly be used by developers of this library. Example are `Parsing enum: {fullyQualifiedName}`.


#### Warning ####

Should be for things which might have unintended consequences.


#### Error ####

Should be used for errors which cannot be recovered from. Parsing an unsupported type for example.



