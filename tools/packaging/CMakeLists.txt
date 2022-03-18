include_guard()

include(GNUInstallDirs)

# Export the Parser library
install(TARGETS Parser EXPORT ParserTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT ParserTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Parser NAMESPACE Tolc::)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Parser
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Add the ways to get the libraries that Parser uses
# NOTE: Do not export imported targets; reuse the same way you got the library
#       https://cmake.org/pipermail/cmake-developers/2016-June/028673.html
# NOTE: Why are private libraries needed?
#       https://cmake.org/pipermail/cmake/2016-August/064054.html
install(FILES
  ${modules}/LLVMUtilities.cmake
  ${modules}/ConanHelpers.cmake
  ${modules}/IncludePathHelpers.cmake
  ${CMAKE_SOURCE_DIR}/tools/conan_profiles/${CMAKE_SYSTEM_NAME}/clang.in
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Parser)

if(NOT llvm_entry_SOURCE_DIR)
  message(FATAL_ERROR "The path to LLVM needs to be set. It is assumed to be in the variable llvm_entry_SOURCE_DIR")
endif()

if(NOT llvm_entry_version)
  message(FATAL_ERROR "The version of LLVM needs to be set. It is assumed to be in the variable llvm_entry_version")
endif()

# Make sure the system headers are exported
# NOTE: They are meant to mirror the directory structure found in the LLVM project.
#       This way, scripts can be reused in consuming projects
if(NOT MSVC)
  install(DIRECTORY
    "${llvm_entry_SOURCE_DIR}/include/c++/v1"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/Parser/include/c++)
endif()
install(DIRECTORY
  "${llvm_entry_SOURCE_DIR}/lib/clang/${llvm_entry_version}/include"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/Parser/lib/clang/${llvm_entry_version})
# LLVM License file
install(FILES ${CMAKE_SOURCE_DIR}/tools/packaging/LLVMLicense.txt
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/Parser/lib/clang/${llvm_entry_version})

configure_file(${CMAKE_SOURCE_DIR}/cmake/packaging/ParserConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ParserConfig.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ParserConfig.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Parser)

# Add License
install(
  FILES
    ${CMAKE_SOURCE_DIR}/COPYRIGHT.md
    ${CMAKE_SOURCE_DIR}/LICENSE.md
  DESTINATION
    .)

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
