cmake_minimum_required(VERSION 3.16)

project(FileManager)

# Set C++ standard and Qt-specific options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set UI search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# Find Qt packages
find_package(Qt6 COMPONENTS 
    Core
    Widgets
    Test
    REQUIRED
)

# Define library sources
set(LIB_SOURCES
    src/filemanager.cpp
    src/mainwindow.cpp
)

set(LIB_HEADERS
    src/mainwindow.h
    src/filemanager.h
)

set(UI_FILES
    src/ui/mainwindow.ui
)

# Create the library
add_library(FileManagerLib STATIC
    ${LIB_SOURCES}
    ${LIB_HEADERS}
    ${UI_FILES}
)

# Set up library properties
target_link_libraries(FileManagerLib
    PUBLIC 
        Qt6::Core
        Qt6::Widgets
)

target_include_directories(FileManagerLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Create main executable
add_executable(FileManager 
    src/main.cpp
)

target_link_libraries(FileManager
    PRIVATE
        FileManagerLib
)

# Testing setup
enable_testing()

add_executable(FileManagerTest
    tests/filemanager_test.cpp
)

target_link_libraries(FileManagerTest
    PRIVATE
        FileManagerLib
        Qt6::Test
)

# Add include paths for tests
target_include_directories(FileManagerTest 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Enable automoc and set the correct working directory for tests
set_target_properties(FileManagerTest
    PROPERTIES
        AUTOMOC ON
        AUTOMOC_MOC_OPTIONS "-I${CMAKE_CURRENT_SOURCE_DIR}/tests"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

add_test(NAME FileManagerTest COMMAND FileManagerTest)