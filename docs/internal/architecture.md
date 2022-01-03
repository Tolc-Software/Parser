# Architecture #

![Parser Architecture](Parser.png "Parser Architecture")

The architecture roughly follows the figure above. The `code` is supplied by the user, and passes through a `Frontend` and a `Consumer`. Both of these are entry points from where various clang tools can be built. Since we are mainly interested in the actual `AST` ([Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)) of the code, we must use the third and last entry point, the `Visitor`. The `Visitor` is a class with a set of functions, all beginning with `Visit` followed by what that function should be called on. E.g. `VisitNamespaceDecl` gets called whenever the `Visitor` encounters a `C++` `namespace` within the `AST`. The function will then take the `AST` node as input.

Whenever an appropriate `AST` node is found, it is passed on to a `Builder`, which converts it to `IRProxy`. `IRProxy` (or `Intermediate Represantation Proxy`) is continuously built by the `Visitor` functions with as much information as possible from each node. When all the `code` has been examined, the `Visitor` will run its destructor, which will be used to take all the `IRProxy` and put it through another set of builders, which will produce the final `IR`.

## Good to know ##

* The `AST` nodes (or clang `decl`s) gets destroyed before the destructor of the `Visitor` is called, therefore they cannot be a part of the `IRProxy`.

* The clang `decl`s use an inheritance scheme, where you can choose to visit more "specialized" `decl`, or even the base (simply `decl`). E.g. `FunctionDecl` covers all functions, but `CXXMethodDecl` is a class function.

