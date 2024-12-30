# EWI Targets
add_library(basic_id basic_id.cpp)
add_executable(test_basic_id basic_id.t.cpp)
target_link_libraries(test_basic_id PRIVATE basic_id)
add_test(NAME basic_id.t COMMAND test_basic_id)


add_library(entry entry.cpp)
target_link_libraries(entry PUBLIC string_flattener)
add_executable(test_entry entry.t.cpp)
add_dependencies(test_entry entry)
target_link_libraries(test_entry PRIVATE entry)
add_test(NAME entry.t COMMAND test_entry)


add_library(record record.cpp) 
target_link_libraries(record
    PUBLIC
    ${exception}
    entry
    padded_view
)
add_executable(test_record record.t.cpp)
add_dependencies(test_record record)
target_link_libraries(test_record PRIVATE record)
add_test(NAME record.t COMMAND test_record)


add_library(employee_record employee_record.cpp)
target_link_libraries(employee_record PUBLIC 
    basic_id
    record
    string_flattener
    ${exception}
)
add_executable(test_employee_record employee_record.t.cpp)
target_link_libraries(test_employee_record PRIVATE employee_record ${exception})
add_test(NAME employee_record.t COMMAND test_employee_record)


add_library(survey survey.cpp)
target_link_libraries(survey 
    PUBLIC 
    ${exception}
    basic_id
    entry
)
add_executable(test_survey survey.t.cpp)
target_link_libraries(test_survey PRIVATE survey)
add_test(
    NAME survey.t 
    COMMAND test_survey
    "${PROJECT_SOURCE_DIR}/test_resources/ewi/eeo_profile.txt"
)


add_library(metrics metrics.cpp)
target_link_libraries(metrics PUBLIC 
    Eigen3::Eigen
    Matplot++::matplot
)
add_executable(test_metrics metrics.t.cpp)
target_link_libraries(test_metrics PRIVATE metrics entry record)
add_test(NAME metrics.t COMMAND test_metrics)

