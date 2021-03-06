project(UPReader)

cmake_minimum_required(VERSION 2.8.8)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
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
                 src/filteredfeedmodel.cpp
                 src/article.cpp
                 src/articlelist.cpp
                 src/contentmodel.cpp
                 src/feedmodel.cpp
                 src/feedparser.cpp
                 src/subscription.cpp
                 src/manager.cpp
                 src/upreader.cpp
                 src/upreaderapp.cpp
                 src/main.cpp)

set(upreader_qml src/qml/main.qml
                 src/qml/ArticleListView.qml
                 src/qml/ArticleView.qml
                 src/qml/ReaderView.qml
                 src/qml/StoryDelegate.qml
                 src/qml/SubItem.qml
                 src/qml/SubList.qml
                 src/qml/WebAuth.qml)

add_custom_target(QmlFiles ALL ${CMAKE_COMMAND}
                  SOURCES ${upreader_qml})

qt5_add_resources(upreader_resources qml.qrc)

# Now make sure all files get to the right place
add_executable(UPReader ${upreader_src} ${upreader_resources})

qt5_use_modules(UPReader Quick Qml Widgets Core Xml)

INSTALL(TARGETS UPReader
    RUNTIME DESTINATION bin)

install(PROGRAMS up-reader.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
