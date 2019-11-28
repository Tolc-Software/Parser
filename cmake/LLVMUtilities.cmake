include_guard()

function(getClangTargets)
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    include(FetchContent)

    FetchContent_Declare(
      llvm_download
      URL http://releases.llvm.org/9.0.0/clang+llvm-9.0.0-aarch64-linux-gnu.tar.xz
    )

    FetchContent_GetProperties(llvm_download)
    if(NOT ${llvm_download_POPULATED})
      message(STATUS "LLVM source code not downloaded. Downloading...")
      FetchContent_Populate(llvm_download)
    endif()

    message(STATUS "LLVM source dir: ${llvm_download_SOURCE_DIR}")

    set(Clang_ROOT ${llvm_download_SOURCE_DIR})
    find_package(Clang CONFIG PATHS ${Clang_ROOT} REQUIRED NO_DEFAULT_PATH)
  endif()
endfunction()
