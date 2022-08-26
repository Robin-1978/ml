cmake_minimum_required(VERSION 3.15)

project(Organism)

set(CMAKE_CXX_STANDARD 17)



#FetchContent_Declare(OpenCV
#    GIT_REPOSITORY "https://github.com/opencv/opencv.git"
#    GIT_TAG "4.6.0"
#)
#FetchContent_MakeAvailable(OpenCV)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(include)
file(GLOB SOURCES "src/*.cc")

add_executable(org ${SOURCES})

include(FetchContent)
FetchContent_Declare(fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG "9.0.0"
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(org ${OpenCV_LIBS} fmt)