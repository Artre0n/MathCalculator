include("C:/Users/ammin/Source/Repos/MathCalculator/build/.qt/QtDeploySupport-Release.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/MathCalculator-plugins-Release.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/ammin/Source/Repos/MathCalculator/build/Release/MathCalculator.exe"
    GENERATE_QT_CONF
)
