cmake_minimum_required(VERSION 3.0.0)

if(UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OSX 1)
endif()
