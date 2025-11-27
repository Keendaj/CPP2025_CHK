cmake_minimum_required(VERSION 3.16)
project(CalculatorRPN VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/Calculator.cpp
    ${CMAKE_SOURCE_DIR}/source/DllLoader.cpp
    ${CMAKE_SOURCE_DIR}/source/Tests/Test.cpp
    ${CMAKE_SOURCE_DIR}/source/utils/Exceptions/CalculatorException.cpp
    ${CMAKE_SOURCE_DIR}/source/utils/Exceptions/Math/MathException.cpp
    ${CMAKE_SOURCE_DIR}/source/utils/Exceptions/DLL/DLLException.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/source/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Tests
)

add_executable(calculator ${SOURCES})

add_custom_command(TARGET calculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/source/plugins
    $<TARGET_FILE_DIR:calculator>/plugins
)
