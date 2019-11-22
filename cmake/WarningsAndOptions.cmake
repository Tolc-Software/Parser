include_guard()

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the standard warnings
add_library(project_warnings INTERFACE)

# Set warning flags for different compilers
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  target_compile_options(project_warnings
                         INTERFACE /W4
                                   /WX
                                   "/permissive-"
                                   /w14640
                                   /w14242
                                   /w14254
                                   /w14263
                                   /w14265
                                   /w14287
                                   /we4289
                                   /w14296
                                   /w14311
                                   /w14545
                                   /w14546
                                   /w14547
                                   /w14549
                                   /w14555
                                   /w14619
                                   /w14826
                                   /w14905
                                   /w14906
                                   /w14928)
else()
  # Either gcc or Clang

  option(LIBCPP "Build with libc++" FALSE)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
  option(ENABLE_ASAN "Enable address sanitizer" FALSE)

  if(ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(project_options INTERFACE --coverage)
  endif()

  if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
  endif()

  target_compile_options(project_warnings
                         INTERFACE -Wall
                                   -Werror # treat all warnings as errors
                                   -Wextra # reasonable and standard
                                   -Wshadow # warn the user if a variable
                                            # declaration shadows one from a
                                            # parent context
                                   -Wnon-virtual-dtor # warn the user if a class
                                                      # with virtual functions
                                                      # has a non-virtual
                                                      # destructor. This helps
                                                      # catch hard to track down
                                                      # memory errors
                                   -Wold-style-cast # warn for c-style casts
                                   -Wcast-align # warn for potential performance
                                                # problem casts
                                   -Wunused # warn on anything being unused
                                   -Woverloaded-virtual # warn if you overload
                                                        # (not override) a
                                                        # virtual function
                                   -Wpedantic # warn if non-standard C++ is used
                                   -Wconversion # warn on type conversions that
                                                # may lose data
                                   -Wsign-conversion # warn on sign conversions
                                   -Wnull-dereference # warn if a null
                                                      # dereference is detected
                                   -Wdouble-promotion # warn if float is
                                                      # implicit promoted to
                                                      # double
                                   -Wdate-time # warn on use of time macros
                                   -Wformat=2 # warn on security issues around
                                              # functions that format output (ie
                                              # printf)
                         )

  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    target_compile_options(project_warnings
                           INTERFACE -Wmisleading-indentation # warn if
                                                              # identation
                                                              # implies blocks
                                                              # where blocks do
                                                              # not exist
                                     -Wduplicated-cond # warn if if / else chain
                                                       # has duplicated
                                                       # conditions
                                     -Wduplicated-branches # warn if if / else
                                                           # branches have
                                                           # duplicated code
                                     -Wlogical-op # warn about logical
                                                  # operations being used where
                                                  # bitwise were probably wanted
                                     -Wuseless-cast # warn if you perform a cast
                                                    # to the same type
                           )
    # filesystem
    target_link_libraries(project_options INTERFACE stdc++fs)
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(project_warnings
                           INTERFACE -Wduplicate-enum
                                     -fdiagnostics-absolute-paths # To avoid
                                                                  # confusion
                                                                  # about which
                                                                  # file is
                                                                  # built
                           )
    # If clang with libc++ no filesystem is required
    if(LIBCPP)
      target_compile_options(project_options INTERFACE -stdlib=libc++)
    else()
      target_link_libraries(project_options INTERFACE stdc++fs)
    endif()
    target_compile_options(project_warnings INTERFACE -Wduplicate-enum)
  endif()
endif()
