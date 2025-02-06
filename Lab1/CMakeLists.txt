cmake_minimum_required(VERSION 3.10)

project(QuadraticEquationSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(QuadraticEquationSolver 
    main.cpp 
    solution.cpp
)

target_include_directories(QuadraticEquationSolver PRIVATE ${CMAKE_SOURCE_DIR})