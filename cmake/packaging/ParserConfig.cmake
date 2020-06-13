include(CMakeFindDependencyMacro)
find_dependency(IR)

include("${CMAKE_CURRENT_LIST_DIR}/LLVMUtilities.cmake")
# Download if necessary and get the clang libs Defines
# llvm_entry_{SOURCE_DIR,INCLUDE_DIR} variables
set(llvm_entry_version 10.0.0)
setup_llvm(NAME "LLVM_ENTRY" VERSION ${llvm_entry_version})

# Macro to get downloaded Clang and llvm packages
get_clang_and_llvm(PATH ${llvm_entry_SOURCE_DIR})

include("${CMAKE_CURRENT_LIST_DIR}/ConanHelpers.cmake")
run_conan(REQUIRES
  CTRE/v2.8.2@ctre/stable)

include("${CMAKE_CURRENT_LIST_DIR}/ParserTargets.cmake")
