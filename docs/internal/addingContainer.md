# How to add a new container #

This document describes how to add support for parsing a new type of standard library container (or anything that can contain types).

1. In the [IntermediateRepresentation repo](https://github.com/srydell/IntermediateRepresentation), start by adding an `enum` value to `IR::ContainerType` corresponding to your new container.
2. Add a test in the root of the `tests` directory. The naming convention is the plural version of the container name. Use another container test as a starting point. For example `lists.cpp`.
3. Change the `code` block so that the container is used.
4. Add the test to the `CMakeLists.txt` file in the `tests` directory.
5. Run the test with a breakpoint in `Helpers/Type/Matchers/container.cpp` to see what the internal string to match for is (e.g. "std::list<int>").
6. Write a regex pattern that matches the internal string and function that uses the regex. Follow the convention used in the rest of `Helpers/Type/Matchers/container.cpp`.
7. Use that function in the `Helpers::Type::Matchers::getContainerType` function.
8. Add any more tests you'd like.

