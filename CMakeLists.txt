CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(bujo)
SET(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(BUILD_TESTING ON)

enable_testing()

find_package(argh CONFIG REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(bujo_lib
	src/config.cpp
	src/date.cpp
	src/documents.cpp
	src/exec.cpp
	src/path.cpp
	src/span.cpp
)
target_include_directories(bujo_lib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_link_libraries(bujo_lib PUBLIC
	argh
	date::date
	date::date-tz
	fmt::fmt
	nlohmann_json::nlohmann_json
	pantor::inja
)

add_executable(bujo src/main.cpp)
target_link_libraries(bujo PRIVATE bujo_lib)

install(TARGETS bujo
	RUNTIME DESTINATION bin
)

if (BUILD_TESTING)
	find_package(GTest CONFIG REQUIRED)

	add_executable(bujo_tests
		tests/test_config.cpp
		tests/test_date.cpp
		tests/test_path.cpp
		tests/test_span.cpp
		tests/test_templates.cpp
	)
	target_link_libraries(bujo_tests PRIVATE
		bujo_lib
		GTest::gtest
		GTest::gtest_main
	)
	target_compile_definitions(bujo_tests PRIVATE
		BUJO_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/data"
	)
	include(GoogleTest)
	gtest_discover_tests(bujo_tests)
endif()
