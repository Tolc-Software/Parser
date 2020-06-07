include_guard()

# Populate the input variable with a string/list with the platform specific
# includes. Example: get_system_include(LLVM_DIRECTORY ${my_llvm_path} LLVM_VERSION 10.0.0 VARIABLE my_includes)
# message(STATUS "My includes are: ${my_includes}")
function(get_system_include)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues LLVM_DIRECTORY LLVM_VERSION VARIABLE)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})
  if(NOT ARG_LLVM_DIRECTORY OR NOT ARG_LLVM_VERSION)
    message(FATAL_ERROR "Not enough information about LLVM")
  endif()
  if(NOT ARG_VARIABLE)
    message(FATAL_ERROR "VARIABLE must be set.")
  endif()
  # We are using the standard library shipped with the downloaded llvm (libc++) for include paths in the parsing
  # But we also need some platform specific code which cannot be assumed
  # These are copied from the platform and added to source control
  # You can find the include paths by typing: $ /path/to/llvm/imported/clang -v -x c++ -
  # These correspond to the way they appear in the command above (this is necessary because they use #include_next)
  set(system_include "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(system_include
      "${ARG_LLVM_DIRECTORY}/include/c++/v1"
      "/usr/local/include"
      "${ARG_LLVM_DIRECTORY}/lib/clang/10.0.0/include"
      "/usr/include")
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Darwin)
    set(system_include
      "${ARG_LLVM_DIRECTORY}/include/c++/v1"
      "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"
      "${ARG_LLVM_DIRECTORY}/lib/clang/10.0.0/include"
      "/usr/local/include"
      "/System/Library/Frameworks"
      "/Library/Frameworks")
  else()
    message(FATAL_ERROR "Unsupported platform for now.")
  endif()
  set(${ARG_VARIABLE}
      ${system_include}
      PARENT_SCOPE)
endfunction()

# Format the INCLUDES Example: format_includes(VARIABLE my_formated_includes
# INCLUDES my_includes) message(STATUS "${my_includes}") ---
# /usr/include;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
# message(STATUS "${my_formated_includes}") --- "-I/usr/include",
# "-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include" NOTE: If
# you want system includes (-isystem) use the SYSTEM flag
function(format_includes)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues SYSTEM)
  set(singleValues VARIABLE)
  set(multiValues INCLUDES)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_VARIABLE)
    message(FATAL_ERROR "VARIABLE must be set.")
  endif()

  set(include_flag "-I")
  if(ARG_SYSTEM)
    set(include_flag "-isystem")
  endif()

  set(temp "")
  # format as: "-Iinclude[0]", "-Iinclude[1]", ...
  foreach(include ${ARG_INCLUDES})
    if(NOT temp)
      set(temp "\"${include_flag}${include}\"")
    else()
      string(APPEND temp ", \"${include_flag}${include}\"")
    endif()
  endforeach()
  set(${ARG_VARIABLE}
      ${temp}
      PARENT_SCOPE)
endfunction()

# Tests: format_includes(VARIABLE bla INCLUDES "Hi;Ho") message(STATUS ${bla})

# get_platform_specific_includes(VARIABLE includes) message(STATUS
# "${includes}") format_includes(VARIABLE bla INCLUDES ${includes})
# message(STATUS ${bla}) format_includes(VARIABLE bla INCLUDES ${includes}
# SYSTEM) message(STATUS ${bla})
