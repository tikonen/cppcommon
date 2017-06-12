cmake_minimum_required(VERSION 3.0.0)
project(common)

if(CMAKE_VERSION VERSION_LESS 3.0.0)
    message(FATAL_ERROR "You must use CMake 3.0.0 or greater.")
endif()

include(cmake/platform.cmake)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(_src_root_path "${PROJECT_SOURCE_DIR}/src")

file(
    GLOB_RECURSE _source_list
    LIST_DIRECTORIES false
    "${_src_root_path}/*.c*"
	"${_src_root_path}/*.h*"
)

file(
    GLOB_RECURSE _include_list
    LIST_DIRECTORIES false
    "${PROJECT_SOURCE_DIR}/include/*.h*"
)

set(_target "common")

add_library(${_target} STATIC ${_source_list} ${_include_list})

target_include_directories(${_target} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(OSX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif()

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
	# built as subproject, skip testing
	set(TESTING FALSE)
else()
	set(TESTING TRUE)
endif()

if(NOT TESTING)
	return()
endif()

###
### Unit testing
###

find_package(GTest)

if(GTEST_FOUND)
	# always link with absolute path, even implicit directories. avoids problems with
	# running targets on IDE when dll's are not in the environment search path.
	cmake_policy(SET CMP0060 NEW)

	enable_testing()

	set(_test_runner_target "testRunner")

	add_executable(${_test_runner_target} 
		"${PROJECT_SOURCE_DIR}/tests/gtest_main.cpp"
		"${PROJECT_SOURCE_DIR}/tests/test_events.cpp"
		)

	target_include_directories(${_test_runner_target} PRIVATE ${GTEST_INCLUDE_DIRS})
	target_link_libraries(${_test_runner_target} ${GTEST_LIBRARIES} ${_target})

	add_test(
		NAME EventListener
		COMMAND ${_test_runner_target} "--gtest_filter" "EventListener.*"
	)
else()
	message("Set GTEST_ROOT to point to the GTest installation location")
endif(GTEST_FOUND)
