# Этот файл лежит внутри папки tests/
add_executable(MathCalculatorTests
    main_test.cpp
    complex_tests.cpp
    polynom_tests.cpp
    matrix_tests.cpp
)

target_include_directories(MathCalculatorTests PRIVATE
    ../core
    ../gui
    ../gui/Matrix
    ../gui/Polynom
    ../gui/Complex
    .
)

target_link_libraries(MathCalculatorTests PRIVATE Qt6::Core)

add_test(NAME MathCalculatorTests COMMAND MathCalculatorTests)