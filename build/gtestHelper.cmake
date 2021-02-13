# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system
# 

# see GitHub documentation of GoogleTest framework: https://github.com/google/googletest/blob/master/googletest/docs/advanced.md

if (DEFINED GTEST_FILTER)
    list(APPEND GTEST_ARGS_LIST "--gtest_filter=${GTEST_FILTER}")
endif()

if (GTEST_COLOR)
    list(APPEND GTEST_ARGS_LIST "--gtest_color=${GTEST_COLOR}")
endif()

if (GTEST_RUN_DISABLED)
    list(APPEND GTEST_ARGS_LIST "--gtest_also_run_disabled_tests")
endif()

if (DEFINED GTEST_REPEAT)
    list(APPEND GTEST_ARGS_LIST "--gtest_repeat=${GTEST_REPEAT}")
endif()

if (DEFINED GTEST_BRIEF)
    list(APPEND GTEST_ARGS_LIST "--gtest_brief=${GTEST_BRIEF}")
endif()

if (GTEST_SHUFFLE)
    list(APPEND GTEST_ARGS_LIST "--gtest_shuffle")
endif()

if (DEFINED GTEST_PRINT_TIME)
    list(APPEND GTEST_ARGS_LIST "--gtest_print_time=${GTEST_PRINT_TIME}")
endif()

if (DEFINED GTEST_OUTPUT)
    list(APPEND GTEST_ARGS_LIST "--gtest_output=${GTEST_OUTPUT}")
endif()

if (GTEST_LIST_TESTS)
    list(APPEND GTEST_ARGS_LIST "--gtest_list_tests")
endif()

if (GTEST_FAIL_FAST)
    list(APPEND GTEST_ARGS_LIST "--gtest_fail_fast")
endif()

if (DEFINED GTEST_PRINT_UTF8)
    list(APPEND GTEST_ARGS_LIST "--gtest_print_utf8=${GTEST_PRINT_UTF8}")
endif()

if (DEFINED GTEST_RANDOM_SEED)
    list(APPEND GTEST_ARGS_LIST "--gtest_random_seed=${GTEST_RANDOM_SEED}")
endif()


