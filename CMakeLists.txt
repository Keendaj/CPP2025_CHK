cmake_minimum_required(VERSION 3.16)
project(CalculatorRPN VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests
)

list(FILTER SOURCES EXCLUDE REGEX "build/*")
list(FILTER SOURCES EXCLUDE REGEX ".*/dllGenerator/.*")

add_executable(calculator
    ${SOURCES}
)

add_custom_command(TARGET calculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/plugins
    $<TARGET_FILE_DIR:calculator>/plugins
)
