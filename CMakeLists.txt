cmake_minimum_required(VERSION 3.27)
project(PyWrightIDE-CPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(qt.cmake)
include(scintilla-qt.cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)

set(PYWRIGHTIDE_SOURCES
    src/main.cpp
    src/IDEMainWindow.cpp
    src/data/IDESettings.cpp
    src/data/IconThemes.cpp
    src/data/PyWrightCase.cpp
    src/data/PyWrightFolder.cpp
    src/data/PyWrightGameInfo.cpp
    src/data/ThumbnailIconProvider.cpp
    src/widgets/CasePropertiesWidget.cpp
    src/widgets/DirectoryViewWidget.cpp
    src/widgets/FileEditWidget.cpp
    src/widgets/GameDataWidget.cpp
    src/widgets/GameIntroWidget.cpp
    src/widgets/GamePropertiesWidget.cpp
    src/widgets/MainWindowCentralWidget.cpp
    src/widgets/MainWindowStatusBar.cpp
    src/widgets/MainWindowTopToolbar.cpp
    src/dialogs/NewGameDialog.cpp
    src/dialogs/AddExistingCaseDialog.cpp
    src/dialogs/CasePropertiesEditorDialog.cpp
    src/dialogs/IconPickerDialog.cpp
    src/dialogs/OpenGameDialog.cpp
    # Include the header files too otherwise you'll get linking errors
    include/widgets/DirectoryViewWidget.hpp
    include/widgets/FileEditWidget.hpp
    include/widgets/GameDataWidget.hpp
    include/widgets/IDETabWidget.hpp
    include/widgets/MainWindowCentralWidget.hpp
    include/widgets/MainWindowStatusBar.hpp
    include/widgets/MainWindowTopToolbar.hpp
    include/dialogs/NewGameDialog.hpp
    include/dialogs/AddExistingCaseDialog.hpp
    include/dialogs/CasePropertiesEditorDialog.hpp
    include/dialogs/IconPickerDialog.hpp
    include/dialogs/OpenGameDialog.hpp
    include/widgets/CasePropertiesWidget.hpp
    include/widgets/GamePropertiesWidget.hpp
    include/widgets/GameIntroWidget.hpp
    include/IDEMainWindow.hpp
)

qt_add_executable(pywrightide ${PYWRIGHTIDE_SOURCES})

include_directories(pywrightide
    ${INCLUDE_DIRECTORIES}
    include
    ${SCINTILLA_QT_INCLUDE_DIRECTORIES}
)

set_target_properties(pywrightide
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(pywrightide
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        scintilla_qt
)

add_custom_command(TARGET pywrightide POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:pywrightide>/res)
