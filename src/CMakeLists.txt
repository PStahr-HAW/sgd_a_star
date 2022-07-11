
include(FetchContent)
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.zip
)
FetchContent_MakeAvailable(tinyxml2)

include_directories(../include)

# add executable
#set(EXE_NAME a_star_exe)
set(LIB_NAME a_star)
set(CPP_FILES
    a_star.cpp
    node.cpp
    route.cpp
    users.cpp
    utils.cpp)

add_library(${LIB_NAME} ${CPP_FILES})
target_link_libraries(${LIB_NAME} tinyxml2)

install(TARGETS ${LIB_NAME}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/a_star_lib
)

#add_executable(${EXE_NAME} src/main.cpp)
#target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})