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

qt5_add_resources(upreader_image_resources images/images.qrc)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${Qt5Quick_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
)
# We add our source code here
set(upreader_src src/filteredfeedmodel.cpp
                 src/article.cpp
                 src/contentmodel.cpp
                 src/feedmodel.cpp
                 src/feedparser.cpp
                 src/subscription.cpp
                 src/manager.cpp
                 src/upreader.cpp
                 src/upreaderapp.cpp
                 src/main.cpp
                 src/apihelper.cpp
                 src/sqlhelper.cpp
                 src/networkmanager.cpp
                 src/qmlsqltablemodel.cpp
                 src/qmlsqlquerymodel.cpp)

include(src/qml/qml.cmake)

# Now make sure all files get to the right place
add_executable(UPReader ${upreader_src} ${upreader_qml_resources} ${upreader_image_resources})

qt5_use_modules(UPReader Quick Qml Widgets Core Xml Sql)

INSTALL(TARGETS UPReader
    RUNTIME DESTINATION bin)

install(PROGRAMS up-reader.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

