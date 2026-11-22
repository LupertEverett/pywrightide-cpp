set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)

if (Qt6_FOUND)
    # QTextCodec, which Scintilla's Qt port is using, is removed in Qt6. So we need the compat libs here
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat REQUIRED)
endif()

set(SCINTILLA_BASE_DIR thirdparty/scintilla)

set(SCINTILLA_QT_SOURCES
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit/ScintillaEdit.cpp
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit/ScintillaEdit.h
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit/ScintillaDocument.cpp
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit/ScintillaDocument.h
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/PlatQt.cpp
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/PlatQt.h
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/ScintillaEditBase.cpp
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/ScintillaEditBase.h
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/ScintillaQt.cpp
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase/ScintillaQt.h
    ${SCINTILLA_BASE_DIR}/src/AutoComplete.cxx
    ${SCINTILLA_BASE_DIR}/src/AutoComplete.h
    ${SCINTILLA_BASE_DIR}/src/CallTip.cxx
    ${SCINTILLA_BASE_DIR}/src/CallTip.h
    ${SCINTILLA_BASE_DIR}/src/CaseConvert.cxx
    ${SCINTILLA_BASE_DIR}/src/CaseConvert.h
    ${SCINTILLA_BASE_DIR}/src/CaseFolder.cxx
    ${SCINTILLA_BASE_DIR}/src/CaseFolder.h
    ${SCINTILLA_BASE_DIR}/src/CellBuffer.cxx
    ${SCINTILLA_BASE_DIR}/src/CellBuffer.h
    ${SCINTILLA_BASE_DIR}/src/ChangeHistory.cxx
    ${SCINTILLA_BASE_DIR}/src/ChangeHistory.h
    ${SCINTILLA_BASE_DIR}/src/CharacterCategoryMap.cxx
    ${SCINTILLA_BASE_DIR}/src/CharacterCategoryMap.h
    ${SCINTILLA_BASE_DIR}/src/CharacterType.cxx
    ${SCINTILLA_BASE_DIR}/src/CharacterType.h
    ${SCINTILLA_BASE_DIR}/src/CharClassify.cxx
    ${SCINTILLA_BASE_DIR}/src/CharClassify.h
    ${SCINTILLA_BASE_DIR}/src/ContractionState.cxx
    ${SCINTILLA_BASE_DIR}/src/ContractionState.h
    ${SCINTILLA_BASE_DIR}/src/DBCS.cxx
    ${SCINTILLA_BASE_DIR}/src/DBCS.h
    ${SCINTILLA_BASE_DIR}/src/Debugging.h
    ${SCINTILLA_BASE_DIR}/src/Decoration.cxx
    ${SCINTILLA_BASE_DIR}/src/Decoration.h
    ${SCINTILLA_BASE_DIR}/src/Document.cxx
    ${SCINTILLA_BASE_DIR}/src/Document.h
    ${SCINTILLA_BASE_DIR}/src/EditModel.cxx
    ${SCINTILLA_BASE_DIR}/src/EditModel.h
    ${SCINTILLA_BASE_DIR}/src/Editor.cxx
    ${SCINTILLA_BASE_DIR}/src/Editor.h
    ${SCINTILLA_BASE_DIR}/src/EditView.cxx
    ${SCINTILLA_BASE_DIR}/src/EditView.h
    ${SCINTILLA_BASE_DIR}/src/ElapsedPeriod.h
    ${SCINTILLA_BASE_DIR}/src/Geometry.cxx
    ${SCINTILLA_BASE_DIR}/src/Geometry.h
    ${SCINTILLA_BASE_DIR}/src/Indicator.cxx
    ${SCINTILLA_BASE_DIR}/src/Indicator.h
    ${SCINTILLA_BASE_DIR}/src/KeyMap.cxx
    ${SCINTILLA_BASE_DIR}/src/KeyMap.h
    ${SCINTILLA_BASE_DIR}/src/LineMarker.cxx
    ${SCINTILLA_BASE_DIR}/src/LineMarker.h
    ${SCINTILLA_BASE_DIR}/src/MarginView.cxx
    ${SCINTILLA_BASE_DIR}/src/MarginView.h
    ${SCINTILLA_BASE_DIR}/src/Partitioning.h
    ${SCINTILLA_BASE_DIR}/src/PerLine.cxx
    ${SCINTILLA_BASE_DIR}/src/PerLine.h
    ${SCINTILLA_BASE_DIR}/src/Platform.h
    ${SCINTILLA_BASE_DIR}/src/PositionCache.cxx
    ${SCINTILLA_BASE_DIR}/src/PositionCache.h
    ${SCINTILLA_BASE_DIR}/src/Position.h
    ${SCINTILLA_BASE_DIR}/src/RESearch.cxx
    ${SCINTILLA_BASE_DIR}/src/RESearch.h
    ${SCINTILLA_BASE_DIR}/src/RunStyles.cxx
    ${SCINTILLA_BASE_DIR}/src/RunStyles.h
    ${SCINTILLA_BASE_DIR}/src/RunThreads.h
    ${SCINTILLA_BASE_DIR}/src/ScintillaBase.cxx
    ${SCINTILLA_BASE_DIR}/src/ScintillaBase.h
    ${SCINTILLA_BASE_DIR}/src/SciTE.properties
    ${SCINTILLA_BASE_DIR}/src/Selection.cxx
    ${SCINTILLA_BASE_DIR}/src/Selection.h
    ${SCINTILLA_BASE_DIR}/src/SparseVector.h
    ${SCINTILLA_BASE_DIR}/src/SplitVector.h
    ${SCINTILLA_BASE_DIR}/src/Style.cxx
    ${SCINTILLA_BASE_DIR}/src/Style.h
    ${SCINTILLA_BASE_DIR}/src/UndoHistory.cxx
    ${SCINTILLA_BASE_DIR}/src/UndoHistory.h
    ${SCINTILLA_BASE_DIR}/src/UniConversion.cxx
    ${SCINTILLA_BASE_DIR}/src/UniConversion.h
    ${SCINTILLA_BASE_DIR}/src/UniqueString.cxx
    ${SCINTILLA_BASE_DIR}/src/UniqueString.h
    ${SCINTILLA_BASE_DIR}/src/ViewStyle.cxx
    ${SCINTILLA_BASE_DIR}/src/ViewStyle.h
    ${SCINTILLA_BASE_DIR}/src/XPM.cxx
    ${SCINTILLA_BASE_DIR}/src/XPM.h
)

set(SCINTILLA_QT_INCLUDE_DIRECTORIES
    ${SCINTILLA_BASE_DIR}/include
    ${SCINTILLA_BASE_DIR}/src
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit
)

add_library(scintilla_qt STATIC ${SCINTILLA_QT_SOURCES})

target_include_directories(scintilla_qt PRIVATE
    ${SCINTILLA_BASE_DIR}/include
    ${SCINTILLA_BASE_DIR}/src
    ${SCINTILLA_BASE_DIR}/qt
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase
)

target_include_directories(scintilla_qt PUBLIC
    ${SCINTILLA_BASE_DIR}/include
    ${SCINTILLA_BASE_DIR}/src
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEdit
    ${SCINTILLA_BASE_DIR}/qt/ScintillaEditBase
)

target_compile_definitions(scintilla_qt PRIVATE
    -DSCINTILLA_QT=1
    -DSCI_LEXER=1
)

target_link_libraries(scintilla_qt PRIVATE Qt::Widgets)
if (Qt6_FOUND)
    target_link_libraries(scintilla_qt PRIVATE Qt6::Core5Compat)
endif()