cmake_minimum_required(VERSION 3.26)
project(CatchEnemies)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp)
add_executable(CatchEnemies ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(CatchEnemies sfml-system sfml-window sfml-graphics)

