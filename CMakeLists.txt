cmake_minimum_required(VERSION 3.20)

project(workload-index)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories("$ENV{HOME}/.local/include/eigen3")

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Other dependencies
find_package(cpptrace REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
#find_package(cpperrors)  # No .cmake file

set(exception cpptrace cpperrors)

add_library(entry OBJECT src/entry.cpp)
target_link_libraries(entry
    PRIVATE
    Eigen3::Eigen
)
add_executable(entry_test 
    src/entry.t.cpp
    $<TARGET_OBJECTS:entry>
)
add_dependencies(entry_test entry)

add_library(record OBJECT src/record.cpp)
add_dependencies(record entry)
target_link_libraries(record
    PUBLIC
    ${exception}
    Eigen3::Eigen
)

add_executable(record_test 
    src/record.t.cpp
    $<TARGET_OBJECTS:entry>
    $<TARGET_OBJECTS:record>
)
add_dependencies(record_test record)
target_link_libraries(record_test ${exception})

#[[
add_library(metricstats OBJECT src/metricstats.hpp src/metricstats.cpp)
target_link_libraries(metricstats
    PUBLIC
    Eigen3::Eigen
)
]]




