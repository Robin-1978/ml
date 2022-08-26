cmake_minimum_required(VERSION 3.15)

project(Organism)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cc")

add_executable(org ${SOURCES})
target_link_libraries(org ${OpenCV_LIBS})