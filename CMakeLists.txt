cmake_minimum_required(VERSION 3.0.0)
project(common)

if(CMAKE_VERSION VERSION_LESS 3.0.0)
    message(FATAL_ERROR "You must use CMake 3.0.0 or greater.")
endif()

include(cmake/platform.cmake)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(_src_root_path "${CMAKE_SOURCE_DIR}/src")

file(
    GLOB_RECURSE _source_list
    LIST_DIRECTORIES false
    "${_src_root_path}/*.c*"
	"${_src_root_path}/*.h*"
)

file(
    GLOB_RECURSE _include_list
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/include/*.h*"
)

set(_target "common")

add_library(${_target} STATIC ${_source_list} ${_include_list})

target_include_directories(${_target} PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(OSX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif()

###
### Unit testing
###

include(ExternalProject)

set(GTest_version 1.8.0)
set(GTest_url "https://github.com/google/googletest/archive/release-${GTest_version}.zip")
set(GTest_md5 "adfafc8512ab65fd3cf7955ef0100ff5")
set(GTest_BUILD_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/build")
set(GTest_BUILD_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
set(GTest_DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Downloads")

ExternalProject_Add(
  GTest
  URL ${GTest_url}
  URL_MD5 ${GTest_md5}
  PREFIX  ${GTest_BUILD_PREFIX}
  DOWNLOAD_DIR ${GTest_DOWNLOAD_DIR}
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:PATH=${GTest_BUILD_INSTALL_PREFIX}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
)

set(GTEST_ROOT ${GTest_BUILD_INSTALL_PREFIX} CACHE STRING "")

enable_testing()

# Include dirs
set(GTEST_INCLUDE_DIR ${GTEST_ROOT}/include)

# Library
set(GTEST_LIBRARY_PATH ${GTEST_ROOT}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES})
set(GTEST_LIBRARY gtest)
add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GTEST_LIBRARY_PATH} )
add_dependencies(${GTEST_LIBRARY} GTest)

set(_test_runner_target "testRunner")

add_executable(${_test_runner_target} 
	"${CMAKE_SOURCE_DIR}/tests/gtest_main.cpp"
	"${CMAKE_SOURCE_DIR}/tests/test_events.cpp"
	)

target_include_directories(${_test_runner_target} PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(${_test_runner_target} ${GTEST_LIBRARY} ${_target})

add_test(
	NAME EventListener
	COMMAND ${_test_runner_target} "--gtest_filter" "EventListener.*"
	)

if(WIN32)
# tell tests where it finds gtest dll's
#set_tests_properties( EventListener PROPERTIES ENVIRONMENT "PATH=${GTEST_ROOT}/lib;$ENV{PATH}")

# copy dll's to target directory to enable running testRunner in debugger
file(
    GLOB _gtest_dll_list
    LIST_DIRECTORIES false
    "${GTEST_ROOT}/lib/*.dll"
)

add_custom_command(
	TARGET ${_test_runner_target} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${_gtest_dll_list}
			$<TARGET_FILE_DIR:${_test_runner_target}>
)

endif()