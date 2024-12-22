# EWI Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(EWI)
enable_testing()

#================ Top-Config ==================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories("$ENV{HOME}/.local/include/eigen3")
include_directories("src")
option(BUILD_SHARED_LIBS "Build shareable libraries" ON)
#=============== Packages =====================

# Qt config
set(QT_VSN Qt5 CACHE STRING "Sets the Qt version to use.")
list(APPEND 
    CMAKE_PREFIX_PATH
    "$ENV{cpp}/.libraries/Qt/6.8.0/gcc_64" "$ENV{cpp}/.libraries/Qt/5.15.2/gcc_64"
)
if (QT_VSN STREQUAL Qt6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
endif()

## Other dependencies
find_package(cpptrace REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Matplot++ REQUIRED)
#find_package(cpperrors)  # No .cmake file
## Workaround until I figure out how to make my errors
## library a single package.
set(exception cpptrace cpperrors)

#========== TARGETS ================
add_subdirectory(src)
