
message(STATUS "Fetching googletest")
include(FetchContent)
FetchContent_Declare(
 googletest
 URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(../include)

enable_testing()

set(testname unit_test_utils)
add_executable(${testname} ${testname}.cpp)
target_link_libraries(${testname} a_star gtest_main)

include(GoogleTest)
gtest_discover_tests(${testname})