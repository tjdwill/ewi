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

add_library(entry ewi/entry.cpp)
target_link_libraries(entry PUBLIC string_flattener)
add_executable(test_entry ewi/entry.t.cpp)
add_dependencies(test_entry entry)
target_link_libraries(test_entry PRIVATE entry)
add_test(NAME entry.t COMMAND test_entry)

add_library(record ewi/record.cpp) 
target_link_libraries(record
    PUBLIC
    ${exception}
    entry
    padded_view
)
add_executable(test_record ewi/record.t.cpp)
add_dependencies(test_record record)
target_link_libraries(test_record PRIVATE record)
add_test(NAME record.t COMMAND test_record)

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

add_library(survey ewi/survey.cpp)
target_link_libraries(survey PUBLIC ${exception} basic_id)
add_executable(test_survey ewi/survey.t.cpp)
target_link_libraries(test_survey PRIVATE survey)
add_test(
    NAME survey.t 
    COMMAND test_survey "${CMAKE_SOURCE_DIR}/test_resources/ewi/eeo_profile.txt"
)

add_library(metrics ewi/metrics.cpp)
target_link_libraries(metrics PUBLIC Eigen3::Eigen)
add_executable(test_metrics ewi/metrics.t.cpp)
target_link_libraries(test_metrics PRIVATE metrics entry record)
add_test(NAME metrics.t COMMAND test_metrics)

