cmake_minimum_required(VERSION 3.5)
project(a_star VERSION 2022.07.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

add_subdirectory(src)
#set(LIB_NAME a_star)
#set(CPP_FILES src/a_star.cpp src/node.cpp src/users.cpp src/utils.cpp src/route.cpp)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    #cmake_minimum_required(VERSION 3.18)
    # option(A_STAR_BUILD_BENCHMARKS "whether or not benchmarks should be built" OFF)
    # option(A_STAR_BUILD_EXAMPLES   "whether or not examples should be built" ON)
    option(A_STAR_BUILD_TESTS      "whether or not tests should be built" OFF)
    #option(A_STAR_BUILD_DOCS       "whether or not docs should be built" OFF)
    option(A_STAR_BUILD_PACKAGE    "whether or not the package should be built" ON)

    if(A_STAR_BUILD_PACKAGE)
        set(package_files include/ src/ CMakeLists.txt LICENSE)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/a_star-src.zip
            COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/a_star-src.zip --format=zip -- ${package_files}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${package_files})
        add_custom_target(a_star_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/a_star-src.zip)
    endif()

    # if(A_STAR_BUILD_EXAMPLES)
    #     add_subdirectory(examples)
    # endif()
    # if(A_STAR_BUILD_BENCHMARKS)
    #     add_subdirectory(benchmarks)
    # endif()
    if(A_STAR_BUILD_TESTS)
        enable_testing()
        add_subdirectory(test)
    endif()
    # if(A_STAR_BUILD_DOCS)
    #     add_subdirectory(docs EXCLUDE_FROM_ALL)
    # endif()
endif()



# find dependencies
# find_package(tinyxml2_vendor REQUIRED)
# find_package(TinyXML2 REQUIRED)

# set(dependencies
#   TinyXML2
# )

# include_directories(include)

# # add executable
# set(EXE_NAME a_star_exe)
# set(LIB_NAME a_star)
# set(CPP_FILES src/a_star.cpp src/node.cpp src/users.cpp src/utils.cpp src/route.cpp)

# add_library(${LIB_NAME} ${CPP_FILES})
# target_link_libraries(${LIB_NAME} tinyxml2)

# add_executable(${EXE_NAME} src/main.cpp)
# target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})

# set(package_files include/ src/ CMakeLists.txt LICENSE)
# add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip
#     COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${package_files}
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     DEPENDS ${package_files})
# add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip)

# #install(
# #  TARGETS ${EXE_NAME} client
# #  RUNTIME DESTINATION lib/${PROJECT_NAME}
# #)

# install(
#  DIRECTORY include/
#  DESTINATION include/)


# ### testing
# enable_testing()

# set(testname unit_test_utils)
# add_executable(${testname} test/${testname}.cpp)
# target_link_libraries(${testname} ${LIB_NAME} gtest_main)

# include(GoogleTest)
# gtest_discover_tests(${testname})

#file(COPY test/data DESTINATION ${CMAKE_BINARY_DIR}/lib/test)
