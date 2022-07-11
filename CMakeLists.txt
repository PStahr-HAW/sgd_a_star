cmake_minimum_required(VERSION 3.5)
project(sgd_a_star VERSION 2022.07.0 LANGUAGES CXX)

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

include(FetchContent)
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.zip
)
FetchContent_MakeAvailable(tinyxml2)

# ---------------------------------------------------------------------------------------
# Create library
# ---------------------------------------------------------------------------------------
include_directories(include)
set(LIB_NAME a_star)
set(CPP_FILES
    a_star.cpp
    node.cpp
    route.cpp
    users.cpp
    utils.cpp)

add_library(${LIB_NAME} ${CPP_FILES})
add_library(${PROJECT_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})
target_link_libraries(${LIB_NAME} tinyxml2)

install(TARGETS ${LIB_NAME}
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
    DIRECTORY include/
    DESTINATION include/)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    #cmake_minimum_required(VERSION 3.18)
    option(A_STAR_BUILD_TESTS      "whether or not tests should be built" OFF)
    #option(A_STAR_BUILD_DOCS       "whether or not docs should be built" OFF)
    option(A_STAR_BUILD_PACKAGE    "whether or not the package should be built" ON)

    if(A_STAR_BUILD_PACKAGE)
        set(package_files include/ src/ CMakeLists.txt LICENSE)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip
            COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${package_files}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${package_files})
        add_custom_target(a_star_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip)
    endif()
    if(A_STAR_BUILD_TESTS)
        enable_testing()
        add_subdirectory(test)
    endif()
    # if(A_STAR_BUILD_DOCS)
    #     add_subdirectory(docs EXCLUDE_FROM_ALL)
    # endif()
endif()

