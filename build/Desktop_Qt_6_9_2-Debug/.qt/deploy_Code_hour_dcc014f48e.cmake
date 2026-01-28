include("/home/nord/Project_Data/Code_Hours/Code_hour/build/Desktop_Qt_6_9_2-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Code_hour-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtwebengine;qtdeclarative;qtwebengine")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/nord/Project_Data/Code_Hours/Code_hour/build/Desktop_Qt_6_9_2-Debug/Code_hour"
    GENERATE_QT_CONF
)
