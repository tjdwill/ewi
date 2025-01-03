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
    appConstants
    ewiUI
    QtConverter
    # Third-party
    Qt::Widgets
    cpperrors
)
target_include_directories(ewi_controller PUBLIC
    ${MY_CPPERRORS_DIR}
    ${MY_EIGEN_DIR}
)
# Make the App!
add_executable(ewiTracker ewi_tracker.cpp)
target_link_libraries(ewiTracker PRIVATE ewi_controller)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ewiTracker PROPERTY WIN32_EXECUTABLE true)
endif()
