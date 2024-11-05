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
#[[
## Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
]]
## Other dependencies
find_package(cpptrace REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
#find_package(cpperrors)  # No .cmake file
set(exception cpptrace cpperrors)
#========== TARGETS ================
add_subdirectory(src)
