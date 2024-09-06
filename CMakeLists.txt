cmake_minimum_required(VERSION 3.15)
project(IndividualMiniproject)

set(INCLUDE_PATHS external_libraries/boost_1_85_0 external_libraries/Crow-1.2.0-Darwin/include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable extra warnings.
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)
endif()

# TODO: Find a way to output coverage on each recompile, or separate this step out as a "coverage
# only" thing.

set(CMAKE_CXX_FLAGS --coverage)

set(SOURCE_FILES src/Course.cpp src/Department.cpp src/MyFileDatabase.cpp src/RouteController.cpp
                 src/MyApp.cpp src/Globals.cpp
)
set(TEST_FILES
    test/CourseUnitTests.cpp test/DepartmentUnitTests.cpp test/MyFileDatabaseUnitTests.cpp
    test/MyAppUnitTests.cpp test/RouteControllerUnitTests.cpp
)
set(INTEGRATION_TEST_FILES test/RouteControllerIntegrationTest.cpp)

# Main project executable.
add_executable(mini_project src/main.cpp ${SOURCE_FILES})
target_include_directories(
    mini_project PUBLIC ${INCLUDE_PATHS} include /opt/homebrew/Cellar/asio/1.30.2/include
)

# Test executable.
add_executable(mini_project_test ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(
    mini_project_test PUBLIC ${INCLUDE_PATHS} include /opt/homebrew/Cellar/asio/1.30.2/include
)
target_link_libraries(mini_project_test gtest gtest_main)

# Test using Google Test.
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/ff233bdd4cac0a0bf6e5cd45bda3406814cb2796.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
gtest_discover_tests(mini_project_test)

# Lint using cpplint.
find_program(CPPLINT cpplint)
if(CPPLINT)
    add_custom_target(
        cpplint
        COMMAND ${CPPLINT} --filter=-whitespace,-build/include_what_you_use ${SOURCE_FILES}
                ${TEST_FILES} src/main.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cpplint for Google C++ Style Guide compliance"
    )
else()
    message(WARNING "cpplint not found! Skipping style checks.")
endif()
