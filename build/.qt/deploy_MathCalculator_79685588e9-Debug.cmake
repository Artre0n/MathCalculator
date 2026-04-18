include("C:/Users/ammin/Source/Repos/MathCalculator/build/.qt/QtDeploySupport-Debug.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/MathCalculator-plugins-Debug.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/ammin/Source/Repos/MathCalculator/build/Debug/MathCalculator.exe"
    GENERATE_QT_CONF
)
