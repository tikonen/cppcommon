cmake_minimum_required(VERSION 3.0.0)

# always link with absolute path, even implicit directories. avoids problems with
# running targets on IDE when dll's are not in the environment search path.
cmake_policy(SET CMP0060 NEW)

if(UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OSX 1)
endif()
