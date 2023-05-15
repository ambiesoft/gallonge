cmake_minimum_required(VERSION 3.5)

# project() sets a project name and the default project version.
# The LANGUAGES argument tells CMake that the program is written in C++.
project(gallonge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt 6 requires a compiler supporting C++ version 17 or newer.
# Enforcing this by setting the CMAKE_CXX_STANDARD, CMAKE_CXX_STANDARD_REQUIRED variables
# will let CMake print an error if the compiler is too old.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS NetworkAuth)

set(TS_FILES gallonge_ja_JP.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        allonge.h
        allonge.cpp
        ../../gallonge_secret/googleid.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gallonge
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gallonge APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(gallonge SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gallonge
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Finally, target_link_libraries tells CMake that the helloworld executable
# makes use of Qt Core by referencing the Qt6::Core target imported by the find_package() call above.
# This will not only add the right arguments to the linker, but also makes sure that the right
# include directories, compiler definitions are passed to the C++ compiler.
# The PRIVATE keyword is not strictly necessary for an executable target, but it is good practice to specify it.
# If helloworld was a library rather than an executable, then either PRIVATE or PUBLIC should be
# specified (PUBLIC if the library mentions anything from Qt6::Core in its headers, PRIVATE otherwise).
target_link_libraries(gallonge PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(gallonge PRIVATE Qt6::NetworkAuth)

set_target_properties(gallonge PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS gallonge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gallonge)
endif()
