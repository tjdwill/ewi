# UTILS
add_library(utils utils/utils.cpp)
target_link_libraries(utils PUBLIC ${exception})

add_executable(test_utils utils/utils.t.cpp)
target_link_libraries(test_utils PRIVATE utils)
add_test(NAME utils.t COMMAND test_utils)

# EWI

add_library(metricstats ewi/metricstats.cpp)
target_link_libraries(metricstats PUBLIC Eigen3::Eigen)


add_library(entry ewi/entry.cpp)

add_executable(entry_test ewi/entry.t.cpp)
add_dependencies(entry_test entry)
target_link_libraries(entry_test PRIVATE entry)
add_test(NAME entry.t COMMAND entry_test)


add_library(record ewi/record.cpp) 
target_link_libraries(record
    PUBLIC
    ${exception}
    Eigen3::Eigen
    entry
    utils
)

add_executable(record_test ewi/record.t.cpp)
add_dependencies(record_test record)
target_link_libraries(record_test PRIVATE record)
add_test(NAME record.t COMMAND record_test)


add_library(employee_record ewi/employee_record.cpp)
target_link_libraries(employee_record PUBLIC record)

