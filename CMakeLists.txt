cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_FLAGS "-std=c++11 -g")
project(chatting)

set(LIB_LIB 
    yxtwebcpp
    dl
    pthread 
    yaml-cpp)


add_executable(testmessagetype tests/testmessagetype.cpp src/messageprotocol.cpp)
target_link_libraries(testmessagetype ${LIB_LIB})

add_executable(testmessagebody tests/testmessagebody.cpp src/messageprotocol.cpp)
target_link_libraries(testmessagebody ${LIB_LIB})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)