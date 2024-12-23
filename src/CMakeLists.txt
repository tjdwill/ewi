add_subdirectory(utils)
add_subdirectory(ewi)
add_subdirectory(ewiQt)

add_library(ewi_controller ewi_controller.cpp)
target_link_libraries(ewi_controller PUBLIC 
    Qt::Widgets
    employee_record
    metrics
    survey
)
