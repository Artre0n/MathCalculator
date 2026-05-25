cmake_minimum_required(VERSION 3.19)
project(MathCalculator LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(MathCalculator
    WIN32 MACOSX_BUNDLE
    app/main.cpp

    gui/Mainwindow.cpp
    gui/Mainwindow.h

    core/Matrix.h
    core/Polynom.h
    core/Complex.h

    gui/Matrix/MatrixTab.h
    gui/Matrix/MatrixTab.cpp
    gui/Matrix/OperationHistory.h
    gui/Matrix/OperationHistory.cpp

    gui/Polynom/PolynomialTab.h
    gui/Polynom/PolynomialTab.cpp

    gui/Complex/ComplexTab.h
    gui/Complex/ComplexTab.cpp

    resources/MathCalculatorIcon.rc

)

target_include_directories(MathCalculator PRIVATE
    .
    app
    gui
    gui/Complex
    gui/Matrix
    gui/Polynom
    core
)

target_link_libraries(MathCalculator
    PRIVATE
        Qt::Core
        Qt::Widgets
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

enable_testing()

add_subdirectory(tests)

add_custom_target(run-tests
    COMMAND MathCalculatorTests
    DEPENDS MathCalculatorTests
    COMMENT "Running pure C++ tests..."
)
