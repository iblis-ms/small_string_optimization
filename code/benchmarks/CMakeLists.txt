AddBenchmarkTarget(
    TARGET_NAME "SmallStringOptimizationBenchmarks"
    SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/SmallStringOptimizationBenchmarks.cpp"
    PUBLIC_LINK_TARGETS "SmallStringOptimization"
    RESOURCES_TO_COPY_TO_EXE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pan-tadeusz.txt"
    )
