cmake_minimum_required(VERSION 3.15)
include("${CMAKE_CURRENT_SOURCE_DIR}/../build/addTarget.cmake")

project(SmallStringOptimization)

enable_testing()

AddInterfaceTarget(
    TARGET_NAME "SmallStringOptimization"
    PUBLIC_INC_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inteface"
    RESOURCES_TO_COPY_TO_EXE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inteface"
)

add_subdirectory(tests)
add_subdirectory(benchmarks)
