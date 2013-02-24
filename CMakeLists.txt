project(UPReader)

cmake_minimum_required(VERSION 2.8.8)

find_package(Qt5Quick REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)

set(CMAKE_AUTOMOC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${Qt5Quick_INCLUDE_DIRS}
)

# We add our source code here
set(upreader_src src/filteredcontentmodel.cpp
                 src/contentmodel.cpp
                 src/feedmodel.cpp
                 src/feedparser.cpp
                 src/subscription.cpp
                 src/manager.cpp
                 src/upreader.cpp
                 src/main.cpp)

add_custom_target(QmlFiles ALL ${CMAKE_COMMAND}
                  SOURCES src/qml/main.qml
                          src/qml/ContentView.qml
                          src/qml/ReaderView.qml
                          src/qml/StoryDelegate.qml
                          src/qml/SubList.qml
                          src/qml/WebAuth.qml)

# Now make sure all files get to the right place
add_executable(UPReader ${upreader_src})

qt5_use_modules(UPReader Quick Qml Gui Core Xml)
