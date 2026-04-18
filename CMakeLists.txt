cmake_minimum_required(VERSION 3.19)
project(MathCalculator LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавлен Test в COMPONENTS
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Test)

qt_standard_project_setup()

qt_add_executable(MathCalculator
    WIN32 MACOSX_BUNDLE
    main.cpp
    Mainwindow.cpp
    Mainwindow.h
    MatrixTab.h
    MatrixTab.cpp
    Matrix.h
    Polynom.h
    OperationHistory.h
    OperationHistory.cpp
)

target_link_libraries(MathCalculator
    PRIVATE
        Qt6::Core      
        Qt6::Widgets   
)

install(TARGETS MathCalculator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MathCalculator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# =====================================================
# UNIT TESTS
# =====================================================
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    add_executable(TestMatrix
        tests/test_matrix.cpp  
    )

    target_link_libraries(TestMatrix
        PRIVATE
            Qt6::Core
            Qt6::Test
    )
    target_include_directories(TestMatrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(TestMatrix PROPERTIES AUTOMOC ON)
    add_test(NAME TestMatrix COMMAND TestMatrix)

    add_executable(TestPolynom
        tests/test_polynom.cpp
    )
    target_link_libraries(TestPolynom
        PRIVATE
            Qt6::Core
            Qt6::Test
    )
    target_include_directories(TestPolynom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(TestPolynom PROPERTIES AUTOMOC ON)
    add_test(NAME TestPolynom COMMAND TestPolynom)
    
    message(STATUS "✅ Unit tests configured")
endif()