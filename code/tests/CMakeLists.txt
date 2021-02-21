
addTestTarget(
    TARGET_NAME "SmallStringOptimizationTests"
    TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/SmallStringiOptimizationTests.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleStringTests.cpp"
    PUBLIC_LINK_TARGETS "SmallStringOptimization"
    )
