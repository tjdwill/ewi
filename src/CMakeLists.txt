add_subdirectory(utils)
add_subdirectory(ewi)
add_subdirectory(ewiQt)

add_library(ewi_controller ewi_controller.cpp)
target_link_libraries(ewi_controller PUBLIC 
    # utils

    # ewi
    employee_record
    metrics
    survey
    # ewiQt
    ewiUI
    QtConverter
    # Third-party
    Qt::Widgets
    ${exception}
)

add_executable(ewiTracker ewi_tracker.cpp)
target_link_libraries(ewiTracker PRIVATE ewi_controller)
