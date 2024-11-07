# UTILS
add_library(padded_view utils/padded_view.cpp)
target_link_libraries(padded_view PUBLIC ${exception})

add_executable(test_padded utils/padded_view.t.cpp)
target_link_libraries(test_padded PRIVATE padded_view)
add_test(NAME padded_view.t COMMAND test_padded)

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
    padded_view
)

add_executable(record_test ewi/record.t.cpp)
add_dependencies(record_test record)
target_link_libraries(record_test PRIVATE record)
add_test(NAME record.t COMMAND record_test)


add_library(employee_record ewi/employee_record.cpp)
target_link_libraries(employee_record PUBLIC record)

