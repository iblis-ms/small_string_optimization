# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/cmake_add_target
# 

# Set to 1 to run valgrind with test
if (NOT DEFINED ADD_TARGET_VALGRIND)
    set(ADD_TARGET_VALGRIND 0)
endif ()

# Set to 1 to run dr memory with test
if (NOT DEFINED ADD_TARGET_DR_MEMORY)
    set(ADD_TARGET_DR_MEMORY 0)
endif ()

# Set to 1 to run clang address sanitizer with test
if (NOT DEFINED ADD_TARGET_CLANG_ADDRESS_SANITIZER)
    set(ADD_TARGET_CLANG_ADDRESS_SANITIZER 0)
endif ()

if (NOT DEFINED ADD_TARGET_CLANG_MEMORY_SANITIZER)
    set(ADD_TARGET_CLANG_MEMORY_SANITIZER 0)
endif ()

if (NOT DEFINED ADD_TARGET_CLANG_THREAD_SANITIZER)
    set(ADD_TARGET_CLANG_THREAD_SANITIZER 0)
endif ()

if (NOT DEFINED ADD_TARGET_CLANG_UNDEFINED_BEHAVIOR_SANITIZER)
    set(ADD_TARGET_CLANG_UNDEFINED_BEHAVIOR_SANITIZER 0)
endif ()

# custom test run command shall be done by defining addTargetTestRunCommand. See AddTargetTestRunCommand_Valgrind to how it can be done.

function(AddTargetTestRunCommand_Valgrind TEST_TARGET_NAME)
    set(ADD_TARGET_TEST_COMMAND "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $<TARGET_FILE:${TEST_TARGET_NAME}>" PARENT_SCOPE)
    set(ADD_TARGET_TEST_SHELL_COMMAND YES PARENT_SCOPE)
endfunction()

function(AddTargetTestRunCommand_DrMemory TEST_TARGET_NAME)
    set(ADD_TARGET_TEST_COMMAND "drmemory.exe" "-ignore_kernel" "-show_reachable" "-batch" "--" "$<TARGET_FILE:${TEST_TARGET_NAME}>" PARENT_SCOPE)
endfunction()