# UTILS

## PaddedView
add_library(padded_view utils/padded_view.cpp)
target_link_libraries(padded_view PUBLIC ${exception})

add_executable(test_padded utils/padded_view.t.cpp)
target_link_libraries(test_padded PRIVATE padded_view)
add_test(NAME padded_view.t COMMAND test_padded)

## String_Flattener
add_subdirectory(utils/string_flattener)

# EWI

add_library(basic_id ewi/basic_id.cpp)
add_executable(test_basic_id ewi/basic_id.t.cpp)
target_link_libraries(test_basic_id PRIVATE basic_id)
add_test(NAME basic_id.t COMMAND test_basic_id)

add_library(metricstats ewi/metricstats.cpp)
target_link_libraries(metricstats PUBLIC Eigen3::Eigen)


add_library(entry ewi/entry.cpp)
target_link_libraries(entry PUBLIC string_flattener)

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
target_link_libraries(employee_record PUBLIC 
    basic_id
    record
    string_flattener
    ${exception}
)

add_executable(test_employee_record ewi/employee_record.t.cpp)
target_link_libraries(test_employee_record PRIVATE employee_record ${exception})
add_test(NAME employee_record.t COMMAND test_employee_record)
