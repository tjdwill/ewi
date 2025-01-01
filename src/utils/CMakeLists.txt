# UTILS
## PaddedView
add_library(padded_view padded_view.cpp)
target_link_libraries(padded_view PUBLIC cpperrors)
add_executable(test_padded padded_view.t.cpp)
target_link_libraries(test_padded PRIVATE padded_view)
add_test(NAME padded_view.t COMMAND test_padded)

## String_Flattener
add_subdirectory(string_flattener)
