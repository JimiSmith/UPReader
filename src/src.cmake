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

