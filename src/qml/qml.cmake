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

qt5_add_resources(upreader_qml_resources src/qml/qml.qrc)