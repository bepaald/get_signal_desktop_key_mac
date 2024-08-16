cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if (APPLE)
  foreach (HOMEBREWPKG openssl)
    execute_process(COMMAND brew --prefix ${HOMEBREW_PKG} OUTPUT_VARIABLE HOMEBREW_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
  endforeach()
endif()

project(get_encryption_key_mac)

find_package(OpenSSL REQUIRED)

if (APPLE)
  find_library(SECLIB Security)
  if (NOT SECLIB)
    message(FATAL_ERROR "Security framework not found")
  endif()

  find_library(COREF CoreFoundation)
  if (NOT COREF)
    message(FATAL_ERROR "Core foundation framework not found")
  endif()
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cc *.h)
add_executable(get_signal_desktop_key_mac ${SOURCES})

target_link_libraries(get_signal_desktop_key_mac ${SECLIB} ${COREF} OpenSSL::Crypto)
