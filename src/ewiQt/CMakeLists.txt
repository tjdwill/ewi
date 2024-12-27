add_library(QtConverter QtConverter.cpp)
target_link_libraries(QtConverter PUBLIC Qt::Core)
add_executable(testQtConverter QtConverter.t.cpp)
target_link_libraries(testQtConverter PRIVATE QtConverter)
add_test(NAME QtConverter.t COMMAND testQtConverter)


add_library(form form.cpp)
target_link_libraries(form PUBLIC Qt::Widgets Qt::Core)
add_executable(form_app form.t.cpp)
target_link_libraries(form_app PRIVATE form)


add_library(imgViewer imgViewer.cpp)
target_link_libraries(imgViewer PUBLIC
    Qt::Gui
    Qt::Widgets
)
add_executable(imgViewerApp imgViewer.t.cpp)
target_link_libraries(imgViewerApp PRIVATE imgViewer)
#[[
add_test(
    NAME test_imgViewer 
    COMMAND imgViewerApp "${PROJECT_SOURCE_DIR}/test_resources/ewiQt/test_ewi.png"
)
]]


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


add_library(ewiUI ewiUI.cpp)
target_link_libraries(ewiUI PUBLIC
    ewi_views
    form
    imgViewer
    Qt::Core
    Qt::Widgets
    Qt::Gui
)
target_compile_definitions(ewiUI PRIVATE EWI_VERSION="${PROJECT_VERSION}")
add_executable(ewiUIApp ewiUI.t.cpp)
target_link_libraries(ewiUIApp PRIVATE
    ewiUI
    ewi_views
    profileLoader
)
target_compile_definitions(ewiUIApp PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

