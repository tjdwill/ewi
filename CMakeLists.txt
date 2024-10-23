cmake_minimum_required(VERSION 3.20)

project(workload-index)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Other dependencies
find_package(cpptrace REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
#find_package(cpperrors)

add_executable(explore src/explore.hpp src/explore.cpp)
target_link_libraries(explore
    PUBLIC 
    Qt5::Widgets Qt5::Core Qt5::Gui
    cpptrace::cpptrace
    cpperrors
    Eigen3::Eigen
)

#[[
add_library(metricstats OBJECT src/metricstats.hpp src/metricstats.cpp)
target_link_libraries(metricstats
    PUBLIC
    Eigen3::Eigen
)
]]




