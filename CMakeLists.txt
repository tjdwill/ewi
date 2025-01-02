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

#[[
    Environment Variable Check

    In order to try to make this build more "flexible" across file systems, 
    I will define a few variables that the user will be responsible for setting. 
    These variables will have default values that work for me, but the idea is that the user 
    will be able to redefine them as needed.

    * MY_EIGEN_DIR:PATH - The location of the Eigen directory
    * MY_CPPERRORS_DIR:PATH - The location of the cpperrors header (currently just a renamed `cpperrors/exception.hpp` file)
    * MY_QT_DIR:PATH - Where the base Qt is defined. This directory may contain multiple Qt versions.
]]
set(MY_LIB_DIR "$ENV{HOME}/.local/include" CACHE PATH "Where local libraries are stored.")
if(NOT DEFINED MY_EIGEN_DIR)
    set(MY_EIGEN_DIR "${MY_LIB_DIR}/eigen3" CACHE PATH "The location of the Eigen directory")
endif()
if (NOT DEFINED MY_CPPERRORS_DIR)
    set(MY_CPPERRORS_DIR "${MY_LIB_DIR}" CACHE PATH "The location of cpperrors header.")
endif()
if (NOT DEFINED MY_QT_DIR)
    set(MY_QT_DIR "${MY_LIB_DIR}/Qt" CACHE PATH "The location of Qt installations.")
endif()

# allow localized inclusions of in-house components using the `<>` form.
include_directories("src")  

#=============== Packages =====================
# Qt config
set(QT_VSN Qt5 CACHE STRING "Sets the Qt version to use.")

if (NOT DEFINED Qt5_DIR)
    list(APPEND 
    # In practice, it may be better to simply set the `Qt5_DIR` variable on the command-line when
    # dealing with mulitple installations for various architectures.
        CMAKE_PREFIX_PATH
        "${MY_QT_DIR}/6.8.0/gcc_64"
        "${MY_QT_DIR}/5.15.2/gcc_64"
        "${MY_QT_DIR}/5.15.2/msvc2019"
    )
endif()

message("Prefix Path: " ${CMAKE_PREFIX_PATH})

if (QT_VSN STREQUAL Qt6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
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
