add_library(form form.cpp)
target_link_libraries(form PUBLIC Qt::Widgets Qt::Core)
add_executable(form_app form.t.cpp)
target_link_libraries(form_app PRIVATE form)


add_library(profileLoader profileLoader.cpp)
target_link_libraries(profileLoader PUBLIC Qt::Widgets)
add_executable(profileLoader_app profileLoader.t.cpp)
target_link_libraries(profileLoader_app PRIVATE profileLoader)


add_library(userOpsWidget userOpsWidget.cpp)
target_link_libraries(userOpsWidget PUBLIC
    Qt::Widgets
)
add_executable(testUserOpsWidget userOpsWidget.t.cpp)
target_link_libraries(testUserOpsWidget PRIVATE userOpsWidget)


add_library(ewi_views views.cpp)
target_link_libraries(ewi_views PUBLIC
    profileLoader
    userOpsWidget
    Qt::Core
    Qt::Widgets
)
add_executable(ewi_views_app views.t.cpp)
target_link_libraries(ewi_views_app PRIVATE
    ewi_views
    Qt::Widgets
)


add_library(ewi_ui ewi_ui.cpp)
target_link_libraries(ewi_ui PUBLIC
    form
    ewi_views
    Qt::Core
    Qt::Widgets
)
add_executable(ewiApp ewi_ui.t.cpp)
target_link_libraries(ewiApp PRIVATE
    ewi_ui
    ewi_views
    profileLoader
)

