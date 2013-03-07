project(UPReader)

cmake_minimum_required(VERSION 2.8.8)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Concurrent REQUIRED)

set(CMAKE_AUTOMOC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-std=c++11)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${Qt5Quick_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
)

qt5_add_resources(upreader_image_resources images/images.qrc)

include(src/src.cmake)
