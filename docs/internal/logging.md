# Logging #

This file contains how this program uses logging, conventions etc.

## Where/what to log ##

Logging is done **exclusively** in the `visitor` functions (i.e. `VisitNamespaceDecl`, `VisitEnumDecl` etc.). This is to avoid cluttering. It also forces the developer to log at a certain level. Prefer logging `Failed to parse return type: {returnType}` rather than `Failed to parse third level allocator in vector<vector<vector<...>>>`.

## Log configuration ##

Should only be done once as close to the start of the program as possible. Preferably only one time with one function.

### Log level ###

#### Debug ####

Should mostly be used by developers of this library. Example are `Parsing enum: {fullyQualifiedName}`.


#### Warning ####

Should be for things which might have unintended consequences.


#### Error ####

Should be used for errors which cannot be recovered from. Parsing an unsupported type for example.



