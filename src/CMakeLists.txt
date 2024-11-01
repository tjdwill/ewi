add_library(entry entry.cpp)

add_executable(entry_test entry.t.cpp)
add_dependencies(entry_test entry)
target_link_libraries(entry_test PRIVATE entry)
add_test(NAME entry.t COMMAND entry_test)


add_library(record record.cpp) 
target_link_libraries(record
    PUBLIC
    ${exception}
    Eigen3::Eigen
    entry
)

add_executable(record_test record.t.cpp)
add_dependencies(record_test record)
target_link_libraries(record_test PRIVATE record)
add_test(NAME record.t COMMAND record_test)
#[[
add_library(metricstats OBJECT metricstats.hpp metricstats.cpp)
target_link_libraries(metricstats
    PUBLIC
    Eigen3::Eigen
)
]]
