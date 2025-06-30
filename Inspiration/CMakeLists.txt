qt_add_library(Settings STATIC)
qt_add_qml_module(settings
    URI UI_Modules
    VERSION 1.0 
    QML_FILES
        Screens/Settings.qml
)