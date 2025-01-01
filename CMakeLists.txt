# EWI Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(EWI
    VERSION 0.2.0
    DESCRIPTION "A tool to visualize one's workload."
    HOMEPAGE_URL "https://github.com/tjdwill/ewi"
    LANGUAGES CXX
)
enable_testing()

#================ Top-Config ==================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories("$ENV{HOME}/.local/include/eigen3")
include_directories("src")

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
include(FetchContent)
FetchContent_Declare(
  cpperrors
  GIT_REPOSITORY    https://github.com/tjdwill/cpp-errors.git
  GIT_TAG           f3887449389dde51d7c87528ffa802806518ad0e
)
FetchContent_MakeAvailable(cpperrors)
find_package(Matplot++ REQUIRED)

#========== TARGETS ================
add_subdirectory(src)
