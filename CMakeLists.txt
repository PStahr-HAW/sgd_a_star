cmake_minimum_required(VERSION 3.5)
project(a_star)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(FetchContent)
FetchContent_Declare(
 googletest
 URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_Declare(
 tinyxml2
 URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.zip
)
FetchContent_MakeAvailable(googletest tinyxml2)

# find dependencies
# find_package(tinyxml2_vendor REQUIRED)
# find_package(TinyXML2 REQUIRED)

# set(dependencies
#   TinyXML2
# )

include_directories(include)

# add executable
set(EXE_NAME a_star_exe)
set(LIB_NAME a_star)
set(CPP_FILES src/a_star.cpp src/node.cpp src/users.cpp src/utils.cpp src/route.cpp)

add_library(${LIB_NAME} ${CPP_FILES})
target_link_libraries(${LIB_NAME} tinyxml2)

add_executable(${EXE_NAME} src/main.cpp)
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})

set(package_files include/ src/ CMakeLists.txt LICENSE)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip
    COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${package_files}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${package_files})
add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip)

#install(
#  TARGETS ${EXE_NAME} client
#  RUNTIME DESTINATION lib/${PROJECT_NAME}
#)

install(
 DIRECTORY include/
 DESTINATION include/)


### testing
enable_testing()

set(testname unit_test_utils)
add_executable(${testname} test/${testname}.cpp)
target_link_libraries(${testname} ${LIB_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${testname})

#file(COPY test/data DESTINATION ${CMAKE_BINARY_DIR}/lib/test)
