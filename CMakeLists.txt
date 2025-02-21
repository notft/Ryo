cmake_minimum_required(VERSION 3.16)

# Set the correct Qt installation path
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.2/msvc2022_64")
set(Qt6_DIR "C:/Qt/6.8.2/msvc2022_64/lib/cmake/Qt6")

project(FileManager)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt package
find_package(Qt6 COMPONENTS Widgets Core REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core REQUIRED)
endif()

# Add your source files (removed fileanalyzer files)
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/filemanager.cpp
)

set(HEADERS
    src/mainwindow.h
    src/filemanager.h
)

# Create the executable
if(WIN32)
    add_executable(FileManager WIN32 ${SOURCES} ${HEADERS})
else()
    add_executable(FileManager ${SOURCES} ${HEADERS})
endif()

# Link Qt libraries
if (Qt6_FOUND)
    target_link_libraries(FileManager PRIVATE Qt6::Widgets Qt6::Core)
else()
    target_link_libraries(FileManager PRIVATE Qt5::Core)
endif()

# Include directories
target_include_directories(FileManager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)