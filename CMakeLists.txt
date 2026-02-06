CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(bujo)
SET(CMAKE_CXX_STANDARD 17)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(BUILD_TESTING ON)

enable_testing()

add_library(bujo_lib
	src/lib.cpp
)
target_include_directories(bujo_lib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_executable(bujo src/main.cpp)
target_link_libraries(bujo PRIVATE bujo_lib)

if (BUILD_TESTING)
	find_package(GTest CONFIG REQUIRED)

	add_executable(bujo_tests
		tests/test_lib.cpp
	)
	target_link_libraries(bujo_tests PRIVATE
		bujo_lib
		GTest::gtest
		GTest::gtest_main
	)
	include(GoogleTest)
	gtest_discover_tests(bujo_tests)
endif()
