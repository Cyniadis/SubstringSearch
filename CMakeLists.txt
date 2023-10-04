cmake_minimum_required(VERSION 3.5)
project(VectorStudyCase)

# Set the C++ standard to C++11 or later
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# Find the Qt5 package
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(QT_DEPS Qt5::Core Qt5::Widgets )

# Include directories for header files
include_directories(lib/include)
include_directories(gui)

# Use file(GLOB ...) to gather source files using wildcards
# LIBS
file(GLOB SOURCES "lib/src/*.cpp")
file(GLOB HEADERS "lib/include/*.h" "lib/include/*.hpp")
add_library(SearchAlgoLib SHARED ${SOURCES} ${HEADERS})


# Main apps
add_executable(GenerateWordList "app/GenerateWordList.cpp"
    gui/ParametersDialog.ui)
add_executable(SearchAlgoCLI "app/SearchAlgoCLI.cpp")
target_link_libraries(SearchAlgoCLI SearchAlgoLib)

add_executable(SearchAlgoTests "app/SearchAlgoTests.cpp")
target_link_libraries(SearchAlgoTests SearchAlgoLib)



# GUI
file(GLOB GUI_SOURCES "gui/*.cpp" "gui/*.h" "gui/*.ui")
#add_library(gui SHARED ${GUI_SOURCES})
#target_link_libraries(gui ${QT_DEPS})

add_executable(SearchAlgoGUI "app/SearchAlgoGUI.cpp"  ${GUI_SOURCES})
target_link_libraries(SearchAlgoGUI ${QT_DEPS})


# string(REPLACE "-O3" "-O0"  CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
