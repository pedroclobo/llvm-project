# This file sets up a CMakeCache for a Fuchsia toolchain build.

option(FUCHSIA_ENABLE_LLDB "Enable LLDB")

set(LLVM_TARGETS_TO_BUILD X86;ARM;AArch64;RISCV CACHE STRING "")

set(PACKAGE_VENDOR Fuchsia CACHE STRING "")

set(_FUCHSIA_ENABLE_PROJECTS "bolt;clang;clang-tools-extra;lld;llvm;polly")

set(LLVM_ENABLE_DIA_SDK OFF CACHE BOOL "")
set(LLVM_ENABLE_LIBEDIT OFF CACHE BOOL "")
set(LLVM_ENABLE_LIBXML2 OFF CACHE BOOL "")
set(LLVM_ENABLE_PER_TARGET_RUNTIME_DIR ON CACHE BOOL "")
set(LLVM_ENABLE_UNWIND_TABLES OFF CACHE BOOL "")
set(LLVM_ENABLE_Z3_SOLVER OFF CACHE BOOL "")
set(LLVM_ENABLE_ZLIB OFF CACHE BOOL "")
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "")
set(LLVM_USE_RELATIVE_PATHS_IN_FILES ON CACHE BOOL "")
set(LLDB_ENABLE_CURSES OFF CACHE BOOL "")
set(LLDB_ENABLE_LIBEDIT OFF CACHE BOOL "")

# Passthrough stage1 flags to stage1.
set(_FUCHSIA_BOOTSTRAP_PASSTHROUGH
  LLVM_ENABLE_ZLIB
  ZLIB_INCLUDE_DIR
  ZLIB_LIBRARY
  LLVM_ENABLE_ZSTD
  zstd_DIR
  LLVM_ENABLE_LIBXML2
  LibXml2_ROOT
  LLVM_ENABLE_CURL
  LLVM_ENABLE_HTTPLIB
  LLVM_ENABLE_LIBEDIT
  CURL_ROOT
  OpenSSL_ROOT
  httplib_ROOT

  # Deprecated
  CursesAndPanel_ROOT

  CURSES_INCLUDE_DIRS
  CURSES_LIBRARIES
  PANEL_LIBRARIES

  # Deprecated
  LibEdit_ROOT

  LibEdit_INCLUDE_DIRS
  LibEdit_LIBRARIES

  FUCHSIA_ENABLE_LLDB
  LLDB_ENABLE_CURSES
  LLDB_ENABLE_LIBEDIT
  LLDB_ENABLE_PYTHON
  LLDB_EMBED_PYTHON_HOME
  LLDB_PYTHON_HOME
  LLDB_PYTHON_RELATIVE_PATH
  LLDB_TEST_USER_ARGS
  Python3_EXECUTABLE
  Python3_LIBRARIES
  Python3_INCLUDE_DIRS
  Python3_RPATH
  SWIG_DIR
  SWIG_EXECUTABLE
  CMAKE_FIND_PACKAGE_PREFER_CONFIG
  CMAKE_SYSROOT
  LLVM_WINSYSROOT
  LLVM_VFSOVERLAY
)

foreach(variable ${_FUCHSIA_BOOTSTRAP_PASSTHROUGH})
  get_property(is_value_set CACHE ${variable} PROPERTY VALUE SET)
  if(${is_value_set})
    get_property(value CACHE ${variable} PROPERTY VALUE)
    get_property(type CACHE ${variable} PROPERTY TYPE)
    set(BOOTSTRAP_${variable} "${value}" CACHE ${type} "")
    if(FUCHSIA_ENABLE_PGO)
      set(BOOTSTRAP_BOOTSTRAP_${variable} "${value}" CACHE ${type} "")
    endif()
  endif()
endforeach()

set(CLANG_DEFAULT_CXX_STDLIB libc++ CACHE STRING "")
set(CLANG_DEFAULT_LINKER lld CACHE STRING "")
set(CLANG_DEFAULT_OBJCOPY llvm-objcopy CACHE STRING "")
set(CLANG_DEFAULT_RTLIB compiler-rt CACHE STRING "")
set(CLANG_DEFAULT_UNWINDLIB libunwind CACHE STRING "")
set(CLANG_ENABLE_STATIC_ANALYZER OFF CACHE BOOL "")
set(CLANG_PLUGIN_SUPPORT OFF CACHE BOOL "")

set(ENABLE_LINKER_BUILD_ID ON CACHE BOOL "")
set(ENABLE_X86_RELAX_RELOCATIONS ON CACHE BOOL "")

set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")
set(LLVM_ENABLE_BACKTRACES ON CACHE BOOL "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "")
elseif(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "")
endif()

if(APPLE)
  set(COMPILER_RT_ENABLE_IOS OFF CACHE BOOL "")
  set(COMPILER_RT_ENABLE_TVOS OFF CACHE BOOL "")
  set(COMPILER_RT_ENABLE_WATCHOS OFF CACHE BOOL "")
endif()

if(WIN32)
  set(LIBCXX_ABI_VERSION 2 CACHE STRING "")
  set(LIBCXX_ENABLE_ABI_LINKER_SCRIPT OFF CACHE BOOL "")
  set(LIBCXX_ENABLE_SHARED OFF CACHE BOOL "")
  set(BUILTINS_CMAKE_ARGS -DCMAKE_SYSTEM_NAME=Windows CACHE STRING "")
  set(RUNTIMES_CMAKE_ARGS -DCMAKE_SYSTEM_NAME=Windows CACHE STRING "")
  set(LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx" CACHE STRING "")
else()
  set(LIBUNWIND_ENABLE_SHARED OFF CACHE BOOL "")
  set(LIBUNWIND_INSTALL_LIBRARY OFF CACHE BOOL "")
  set(LIBUNWIND_USE_COMPILER_RT ON CACHE BOOL "")
  set(LIBCXXABI_ENABLE_SHARED OFF CACHE BOOL "")
  set(LIBCXXABI_ENABLE_STATIC_UNWINDER ON CACHE BOOL "")
  set(LIBCXXABI_INSTALL_LIBRARY OFF CACHE BOOL "")
  set(LIBCXXABI_USE_COMPILER_RT ON CACHE BOOL "")
  set(LIBCXXABI_USE_LLVM_UNWINDER ON CACHE BOOL "")
  set(LIBCXX_ABI_VERSION 2 CACHE STRING "")
  set(LIBCXX_ENABLE_SHARED OFF CACHE BOOL "")
  set(LIBCXX_ENABLE_STATIC_ABI_LIBRARY ON CACHE BOOL "")
  set(LIBCXX_HARDENING_MODE "none" CACHE STRING "")
  set(LIBCXX_USE_COMPILER_RT ON CACHE BOOL "")
  set(COMPILER_RT_BUILD_LIBFUZZER OFF CACHE BOOL "")
  set(COMPILER_RT_BUILD_PROFILE ON CACHE BOOL "")
  set(COMPILER_RT_BUILD_SANITIZERS OFF CACHE BOOL "")
  set(COMPILER_RT_BUILD_XRAY OFF CACHE BOOL "")
  set(COMPILER_RT_USE_BUILTINS_LIBRARY ON CACHE BOOL "")
  set(COMPILER_RT_DEFAULT_TARGET_ONLY ON CACHE BOOL "")
  set(SANITIZER_CXX_ABI "libc++" CACHE STRING "")
  set(SANITIZER_CXX_ABI_INTREE ON CACHE BOOL "")
  set(SANITIZER_TEST_CXX "libc++" CACHE STRING "")
  set(SANITIZER_TEST_CXX_INTREE ON CACHE BOOL "")
  set(LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind" CACHE STRING "")
  set(RUNTIMES_CMAKE_ARGS "-DCMAKE_OSX_DEPLOYMENT_TARGET=10.13;-DCMAKE_OSX_ARCHITECTURES=arm64|x86_64" CACHE STRING "")
endif()

if(BOOTSTRAP_CMAKE_SYSTEM_NAME)
  set(target "${BOOTSTRAP_CMAKE_CXX_COMPILER_TARGET}")
  if(STAGE2_LINUX_${target}_SYSROOT)
    set(LLVM_BUILTIN_TARGETS "${target}" CACHE STRING "")
    set(BUILTINS_${target}_CMAKE_SYSTEM_NAME Linux CACHE STRING "")
    set(BUILTINS_${target}_CMAKE_BUILD_TYPE Release CACHE STRING "")
    set(BUILTINS_${target}_CMAKE_SYSROOT ${STAGE2_LINUX_${target}_SYSROOT} CACHE STRING "")

    set(LLVM_RUNTIME_TARGETS "${target}" CACHE STRING "")
    set(RUNTIMES_${target}_CMAKE_SYSTEM_NAME Linux CACHE STRING "")
    set(RUNTIMES_${target}_CMAKE_BUILD_TYPE Release CACHE STRING "")
    set(RUNTIMES_${target}_CMAKE_SYSROOT ${STAGE2_LINUX_${target}_SYSROOT} CACHE STRING "")
    set(RUNTIMES_${target}_COMPILER_RT_USE_BUILTINS_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_ENABLE_SHARED OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_INSTALL_LIBRARY OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_ENABLE_SHARED OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_USE_LLVM_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_ENABLE_STATIC_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_INSTALL_LIBRARY OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_ENABLE_SHARED OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_ENABLE_STATIC_ABI_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_ABI_VERSION 2 CACHE STRING "")
    set(RUNTIMES_${target}_LLVM_ENABLE_ASSERTIONS OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind" CACHE STRING "")
    set(RUNTIMES_${target}_SANITIZER_CXX_ABI "libc++" CACHE STRING "")
    set(RUNTIMES_${target}_SANITIZER_CXX_ABI_INTREE ON CACHE BOOL "")
  endif()
endif()

set(BOOTSTRAP_LLVM_ENABLE_LLD ON CACHE BOOL "")
set(BOOTSTRAP_LLVM_ENABLE_LTO ON CACHE BOOL "")

if(FUCHSIA_ENABLE_PGO)
  set(BOOTSTRAP_LLVM_BUILD_INSTRUMENTED ON CACHE BOOL "")

  set(_FUCHSIA_BOOTSTRAP_TARGETS
    generate-profdata
    stage2
    stage2-toolchain-distribution
    stage2-install-toolchain-distribution
    stage2-install-toolchain-distribution-stripped
    stage2-install-toolchain-distribution-toolchain
    stage2-check-all
    stage2-check-clang
    stage2-check-lld
    stage2-check-llvm
    stage2-check-polly
    stage2-test-suite)
  if(FUCHSIA_ENABLE_LLDB)
    list(APPEND _FUCHSIA_ENABLE_PROJECTS lldb)
    list(APPEND _FUCHSIA_BOOTSTRAP_TARGETS
      stage2-check-lldb
      stage2-debugger-distribution
      stage2-install-debugger-distribution
      stage2-install-debugger-distribution-stripped
      stage2-install-debugger-distribution-toolchain)
  endif()
else()
 set(_FUCHSIA_BOOTSTRAP_TARGETS
   check-all
   check-clang
   check-lld
   check-llvm
   check-polly
   llvm-config
   clang
   clang-test-depends
   lld-test-depends
   llvm-test-depends
   test-suite
   test-depends
   toolchain-distribution
   install-toolchain-distribution
   install-toolchain-distribution-stripped
   install-toolchain-distribution-toolchain)
 if(FUCHSIA_ENABLE_LLDB)
   list(APPEND _FUCHSIA_ENABLE_PROJECTS lldb)
   list(APPEND _FUCHSIA_BOOTSTRAP_TARGETS
     check-lldb
     lldb-test-depends
     debugger-distribution
     install-debugger-distribution
     install-debugger-distribution-stripped
     install-debugger-distribution-toolchain)
 endif()
endif()

set(LLVM_ENABLE_PROJECTS ${_FUCHSIA_ENABLE_PROJECTS} CACHE STRING "")
set(CLANG_BOOTSTRAP_TARGETS ${_FUCHSIA_BOOTSTRAP_TARGETS} CACHE STRING "")

get_cmake_property(variableNames VARIABLES)
foreach(variableName ${variableNames})
  if(variableName MATCHES "^STAGE2_")
    list(APPEND EXTRA_ARGS "-D${variableName}=${${variableName}}")
    string(REPLACE "STAGE2_" "" new_name ${variableName})
    string(REPLACE ";" "|" value "${${variableName}}")
    list(APPEND EXTRA_ARGS "-D${new_name}=${value}")
  endif()
endforeach()

# TODO: This is a temporary workaround until we figure out the right solution.
set(BOOTSTRAP_LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind" CACHE STRING "")

set(LLVM_BUILTIN_TARGETS "default" CACHE STRING "")
set(LLVM_RUNTIME_TARGETS "default" CACHE STRING "")

# Setup the bootstrap build.
set(CLANG_ENABLE_BOOTSTRAP ON CACHE BOOL "")
set(CLANG_BOOTSTRAP_EXTRA_DEPS
  builtins
  runtimes
  CACHE STRING "")
if(FUCHSIA_ENABLE_PGO)
  set(CLANG_BOOTSTRAP_CMAKE_ARGS
    ${EXTRA_ARGS}
    -C ${CMAKE_CURRENT_LIST_DIR}/Fuchsia-stage2-instrumented.cmake
    CACHE STRING "")
else()
  set(CLANG_BOOTSTRAP_CMAKE_ARGS
    ${EXTRA_ARGS}
    -C ${CMAKE_CURRENT_LIST_DIR}/Fuchsia-stage2.cmake
    CACHE STRING "")
endif()
