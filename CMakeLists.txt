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

set(_target "common")

add_library(${_target} STATIC ${_source_list})

target_include_directories(${_target} PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(OSX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif()
