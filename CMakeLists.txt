cmake_minimum_required(VERSION 2.8.12)
project(MRSim)

find_package(PythonInterp 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)
pybind11_add_module(MRSim
    MRBrain.cpp
    MRTimedThread.cpp)

set_target_properties(MRSim PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY "../")

