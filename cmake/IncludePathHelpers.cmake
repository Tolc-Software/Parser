include_guard()

# Populate the input variable with a string/list with the platform specific
# includes. Example: get_platform_specific_includes(VARIABLE my_includes)
# message(STATUS "My includes are: ${my_includes}")
function(get_platform_specific_includes)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VARIABLE)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_VARIABLE)
    message(FATAL_ERROR "VARIABLE must be set.")
  endif()
  # Set the include path for the library We are using the standard library
  # shipped with the downloaded llvm for include paths in the parsing
  set(platform_include "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(platform_include "")
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Darwin)
    # You can find the include paths by typing: $ clang -v -x c++ -
    set(platform_include
        "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include;/Library/Developer/CommandLineTools/usr/include/c++/v1"
    )
  else()
    message(FATAL_ERROR "Unsupported platform for now.")
  endif()
  set(${ARG_VARIABLE}
      ${platform_include}
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
