cmake_minimum_required(VERSION 3.0.0)
project(common)

include(cmake/platform.cmake)

set(CMAKE_CXX_STANDARD 11)
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

target_include_directories(${_target} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/exports/)

#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost COMPONENTS regex)

#if(Boost_FOUND)
#    target_include_directories(${_target} PUBLIC ${Boost_INCLUDE_DIR})
#    target_link_libraries(${_target} ${Boost_LIBRARIES})
#endif()

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
	# built as subproject, skip testing
	set(TESTING FALSE)
else()
	set(TESTING TRUE)
endif()

###
### Unit testing
###

if(TESTING)

	find_package(GTest)

	if(GTEST_FOUND)
		# always link with absolute path, even implicit directories. avoids problems with
		# running targets on IDE when dll's are not in the environment search path.
		cmake_policy(SET CMP0060 NEW)

		enable_testing()

		set(_test_runner_target "testRunner")

		file(
			GLOB _test_source_list
			LIST_DIRECTORIES false
			"${PROJECT_SOURCE_DIR}/tests/test_*.cpp"
		)
        if(WIN32)
		          set_source_files_properties(${_test_source_list}
			               PROPERTIES COMPILE_FLAGS "/utf-8")
        endif()
		add_executable(${_test_runner_target} ${_test_source_list})

		target_include_directories(${_test_runner_target} PRIVATE ${GTEST_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/exports/)
		target_link_libraries(${_test_runner_target} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${_target})

    macro(add_gtest _test)
      add_test(
  			NAME ${_test}
  			COMMAND ${_test_runner_target} "--gtest_filter=${_test}.*"
  		)
    endmacro(add_gtest)

    add_gtest(EventListener)
    add_gtest(StringUtil)
    add_gtest(CSVReader)
    add_gtest(BinaryPacker)
	add_gtest(Registry)

	else()
		message("Tests disabled. To enable set GTEST_ROOT to the GTest installation location")
	endif(GTEST_FOUND)
endif(TESTING)

configure_file(${PROJECT_SOURCE_DIR}/CppCommonConfig.h.in ${PROJECT_BINARY_DIR}/exports/CppCommonConfig.h)
