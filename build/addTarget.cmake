# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system
# 
# AddTarget simpliefies adding targets in CMake. For example:
##################### <EXAMPLE> ########################
#   set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
#   set(SRC
#       "${SRC_DIR}/main.cpp"
#       "${SRC_DIR}/ExeA.cpp"
#       "${SRC_DIR}/exeA1/ExeA1.cpp"
#       "${SRC_DIR}/exeA1/ExeA1Sub/ExeA1Sub.cpp"
#       "${SRC_DIR}/exeA2/ExeA2.cpp"
#       )
#   set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")
#   
#   AddTarget(
#      TARGET_NAME "exeA"                      # - target name
#      TARGET_TYPE "EXE"                       # - type of target: "EXE" so target is executable
#      SRC "${SRC}"                            # - source files
#      PRIVATE_LINK_TARGETS "LibraryA"                 # - libraries to link with this target; private linking, so not visible to others
#      PUBLIC_INC_DIRS "${INC_DIR}"            # - path to directories with header files
#      PUBLIC_DEFINES "DEFINE_A" "DEFINE_AA=1" # - defines
#   )
##################### </EXAMPLE> #######################

# Set to 1 to print debug values
if (NOT DEFINED ADD_TARGET_DEBUG)
    set(ADD_TARGET_DEBUG 1)
endif ()


set(CMAKE_MACOSX_RPATH "OFF")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include("${CMAKE_CURRENT_LIST_DIR}/logger.cmake")

set(CONAN_CMAKE_FILE "${CMAKE_CURRENT_LIST_DIR}/conan.cmake") # set path to file

if(NOT EXISTS "${CONAN_CMAKE_FILE}") # if not exist, download it
   logStatus("Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                 "${CONAN_CMAKE_FILE}")
endif()

include("${CONAN_CMAKE_FILE}") 
include("${CMAKE_CURRENT_LIST_DIR}/gtestHelper.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/sanitizerHelper.cmake")


function(initGCovConfig)

    file(REMOVE "${GCOV_CONF_PATH}")
  
    set(GCOV_CONFIG "output = ${GCOV_OUTPUT_PATH}\n")
    set(GCOV_CONFIG "${GCOV_CONFIG}html-details = yes\n")
    set(GCOV_CONFIG "${GCOV_CONFIG}root = ${CMAKE_BINARY_DIR}\n")

    file(WRITE "${GCOV_CONF_PATH}" "${GCOV_CONFIG}")

endfunction()

function(initBenchmarkConfig)

    set(GLOBAL_INTERNAL_BENCHMARK_PROGRAMSS_FILE_PATH  ON  CACHE BOOL "Benchmark program list path")
    file(REMOVE "${GLOBAL_INTERNAL_BENCHMARK_PROGRAMSS_FILE_PATH}")

endfunction()

if (NOT DEFINED GLOBAL_INTERNAL_SETUP)
    set(GLOBAL_INTERNAL_SETUP  ON  CACHE BOOL "Global setup done")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if (NOT WIN32)
            set(GLOBAL_INTERNAL_COMPILE_OPTIONS  "-fPIC"  CACHE INTERNAL "Compilation fla: -fPIC")
        endif()
    endif ()

    if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
        set(GLOBAL_INTERNAL_COMPILER_GCC ON)
    endif()

    if (GCOV_ENABLE)
        initGCovConfig()
    endif ()
    get_filename_component(BUF "${CMAKE_CURRENT_LIST_FILE}"  DIRECTORY)
    set(GLOBAL_INTERNAL_THIS_SCRIPT_DIR  "${BUF}"  CACHE INTERNAL "This sript location")
endif ()

# \brief Creates CMake groups based on folder structure. It only creates groups from folders passing recoursively and returning files that have one of given extensions.
# \param[in] ROOT_PATH Path to location which will be the root of mapping folder to groups,
# \param[out] OUTPUT_FILES Output variable with the given name will be a list of files that have one of given extensions,
# \param[in] PATHS_TO_DIRS Path to locations where recoursive walk shall start,
# \param[in] EXTENSIONS Extensions. Files that have one of given exention will be returned in a list with name given OUTPUT_FILES argument.
function(addGroupsInternal)

    set(OPTIONAL_ARGUMENTS_PATTERN 
        )
    
    set(ONE_ARGUMENT_PATTERN 
        ROOT_PATH 
        OUTPUT_FILES
        ) 
        
    set(MULTI_ARGUMENT_PATTERN 
        PATHS_TO_DIRS 
        EXTENSIONS
        )
    
    CMake_parse_arguments(ARG "${OPTIONAL_ARGUMENTS_PATTERN}" "${ONE_ARGUMENT_PATTERN}" "${MULTI_ARGUMENT_PATTERN}" ${ARGN} )
    list(APPEND OUTPUT_FILES)
    foreach(PATH_DIR IN LISTS ARG_PATHS_TO_DIRS)
        
    
        foreach(EXTENSION IN LISTS ARG_EXTENSIONS)
            file(GLOB_RECURSE PATHS_TO_FILES "${PATH_DIR}/*.${EXTENSION}")
            
            foreach(PATH_TO_FILE IN LISTS PATHS_TO_FILES)
            
                get_filename_component(OUTPUT_DIR "${PATH_TO_FILE}" DIRECTORY)
                file(RELATIVE_PATH RELATIVE "${ARG_ROOT_PATH}" "${OUTPUT_DIR}")
                string(REPLACE  "/" "\\"  OUTPUT_VAR "${RELATIVE}" )
                source_group("${OUTPUT_VAR}" FILES "${PATH_TO_FILE}")
                    
            endforeach()
            
            list(APPEND OUTPUT_FILES "${PATHS_TO_FILES}")
            
        endforeach()
                
    endforeach()
     
    set(${ARG_OUTPUT_FILES} "${OUTPUT_FILES}" PARENT_SCOPE)
    
endfunction()


# \brief Creates CMake groups based on folder structure. It only creates groups from folders where given source files are located. It doesn't go recoursively into folder structures (like function addGroupsInternal).
# \param[in] ROOT_PATH Path to location which will be the root of mapping folder to groups,
# \param[in] PATHS_TO_SRCS Path to source file. Their folder (paths) will be mapped to CMake groups.
function(addSrcGroupsInternal)
    set(OPTIONAL_ARGUMENTS_PATTERN 
        )
    
    set(ONE_ARGUMENT_PATTERN 
        ROOT_PATH 
        ) 
        
    set(MULTI_ARGUMENT_PATTERN 
        PATHS_TO_SRCS
        )
    
    CMake_parse_arguments(ARG "${OPTIONAL_ARGUMENTS_PATTERN}" "${ONE_ARGUMENT_PATTERN}" "${MULTI_ARGUMENT_PATTERN}" ${ARGN} )

    foreach(PATH_TO_SRC IN LISTS ARG_PATHS_TO_SRCS)
        get_filename_component(OUTPUT_DIR "${PATH_TO_SRC}" DIRECTORY)
        file(RELATIVE_PATH RELATIVE  "${ARG_ROOT_PATH}" "${OUTPUT_DIR}")
        string(REPLACE  "/" "\\"  OUTPUT_VAR "${RELATIVE}" )
        source_group("${OUTPUT_VAR}" FILES "${PATH_TO_SRC}")
    endforeach()
endfunction()

# \brief Creates target.
# \param[in] TEST_TARGET Indicates if target is a test target (internal use only),
# \param[in] WIN_DLL_EXPORT_ALL_SYMBOLS Windows: Indicates if all DLL symbols shall be exported,
# \param[in] TARGET_NAME Target name,
# \param[in] TARGET_TYPE Target type: EXE for executable, STATIC for static library, SHARED for shared library, INTERFACE for headers only library,
# \param[in] TARGET_DIR Path to directory where target is defined,
# \param[in] TARGET_OUTPUT_DIR Path where built file be located,
# \param[in] SRC Source files,
# \param[in] PUBLIC_INC_DIRS Arguments to target_include_directories with PUBLIC/INTERFACE visibility,
# \param[in] PRIVATE_INC_DIRS Arguments to target_include_directories with PRIVATE visibility,
# \param[in] PUBLIC_LINK_TARGETS Arguments to target_link_libraries with PUBLIC/INTERFACE visibility,
# \param[in] PRIVATE_LINK_TARGETS Arguments to target_link_libraries with PRIVATE visibility,
# \param[in] PUBLIC_DEFINES Arguments to target_compile_definitions with PUBLIC/INTERFACE visibility,
# \param[in] PRIVATE_DEFINES Arguments to target_compile_definitions with PRIVATE visibility,
# \param[in] RESOURCES_TO_COPY_TO_EXE_DIR Resources that shall be copied to all executable target file location that link this target,
# \param[in] RESOURCES_TO_COPY Similar to RESOURCES_TO_COPY_TO_EXE_DIR, but resources that shall be copied to given location. Format: source_path TO dest_path,
# \param[in] PUBLIC_LINK_OPTIONS Linker arguments with PUBLIC visibility,
# \param[in] PRIVATE_LINK_OPTIONS Linker arguments with PRIVATE visibility,
# \param[in] PUBLIC_COMPILE_OPTIONS Compiler arguments with PUBLIC visibility,
# \param[in] PRIVATE_COMPILE_OPTIONS Compiler arguments with PRIVATE visibility.
function(AddTargetInternal)

    set(OPTIONAL_ARGUMENTS_PATTERN 
        TEST_TARGET
        BENCHMARK_TARGET
        WIN_DLL_EXPORT_ALL_SYMBOLS
        )
    
    set(ONE_ARGUMENT_PATTERN 
        TARGET_NAME 
        TARGET_TYPE 
        TARGET_DIR
        TARGET_OUTPUT_DIR
        ) 
        
    set(MULTI_ARGUMENT_PATTERN 
        SRC 
        
        PUBLIC_INC_DIRS 
        PRIVATE_INC_DIRS 
        
        PUBLIC_LINK_TARGETS 
        PRIVATE_LINK_TARGETS 
        
        PUBLIC_DEFINES
        PRIVATE_DEFINES

        RESOURCES_TO_COPY_TO_EXE_DIR
        RESOURCES_TO_COPY

        PUBLIC_LINK_OPTIONS
        PRIVATE_LINK_OPTIONS

        PUBLIC_COMPILE_OPTIONS
        PRIVATE_COMPILE_OPTIONS

        TEST_SRC
        )
  
    CMake_parse_arguments(ADD_TARGET "${OPTIONAL_ARGUMENTS_PATTERN}" "${ONE_ARGUMENT_PATTERN}" "${MULTI_ARGUMENT_PATTERN}" ${ARGN} )

    if (ADD_TARGET_DEBUG)
        logDebug("--------------------------------------------------------------")
        logDebug("TARGET_NAME=${ADD_TARGET_TARGET_NAME}")
        logDebug("TARGET_TYPE=${ADD_TARGET_TARGET_TYPE}")
        logDebug("TARGET_DIR=${ADD_TARGET_TARGET_DIR}")
        
        logDebug("SRC=${ADD_TARGET_SRC}")
        logDebug("TEST_SRC=${ADD_TARGET_TEST_SRC}")
        
        logDebug("PUBLIC_INC_DIRS=${ADD_TARGET_PUBLIC_INC_DIRS}")
        logDebug("PRIVATE_INC_DIRS=${ADD_TARGET_PRIVATE_INC_DIRS}")
        
        logDebug("PUBLIC_LINK_TARGETS=${ADD_TARGET_PUBLIC_LINK_TARGETS}")
        logDebug("PRIVATE_LINK_TARGETS=${ADD_TARGET_PRIVATE_LINK_TARGETS}")
        
        logDebug("PUBLIC_DEFINES=${ADD_TARGET_PUBLIC_DEFINES}")
        logDebug("PRIVATE_DEFINES=${ADD_TARGET_PRIVATE_DEFINES}")
        
        logDebug("RESOURCES_TO_COPY_TO_EXE_DIR=${ADD_TARGET_RESOURCES_TO_COPY_TO_EXE_DIR}")
        logDebug("RESOURCES_TO_COPY=${ADD_TARGET_RESOURCES_TO_COPY}")
        
        logDebug("PUBLIC_LINK_OPTIONS=${ADD_TARGET_PUBLIC_LINK_OPTIONS}")
        logDebug("PRIVATE_LINK_OPTIONS=${ADD_TARGET_PRIVATE_LINK_OPTIONS}")
        
        logDebug("PUBLIC_COMPILE_OPTIONS=${ADD_TARGET_PUBLIC_COMPILE_OPTIONS}")
        logDebug("PRIVATE_COMPILE_OPTIONS=${ADD_TARGET_PRIVATE_COMPILE_OPTIONS}")
        
        logDebug("TEST_TARGET=${ADD_TARGET_TEST_TARGET}")
        
        logDebug("WIN_DLL_EXPORT_ALL_SYMBOLS=${ADD_TARGET_WIN_DLL_EXPORT_ALL_SYMBOLS}")
        
        logDebug("--------------------------------------------------------------")
    endif ()

  #  set(ADD_TARGET_SRC "${ADD_TARGET_SRC}" "${ADD_TARGET_TEST_SRC}")
    set(ALL_SRC "${ADD_TARGET_SRC}" "${ADD_TARGET_TEST_SRC}")

    if (ADD_TARGET_TEST_TARGET) # only test targets can be excluded/include by match pattern via this script.
        if (ADD_TARGET_TEST_TARGET_INCLUDE)
            # check if include match pattern was defined and this target match to the pattern. If not match, don't create the target.
            if (NOT "${ADD_TARGET_TARGET_NAME}" MATCHES "${ADD_TARGET_TEST_TARGET_INCLUDE}")
                logStatus("Target ${ADD_TARGET_TARGET_NAME} doesn't match to regex: ${ADD_TARGET_TEST_TARGET_INCLUDE}, so the test target wouldn't be built.")
                return()
            endif ()
        endif ()
        # check if exclude match pattern was defined and this target match to the pattern. If match, don't create the target.
        if (ADD_TARGET_TEST_TARGET_EXCLUDE)
            if ("${ADD_TARGET_TARGET_NAME}" MATCHES "${ADD_TARGET_TEST_TARGET_EXCLUDE}")
                logStatus("Target ${ADD_TARGET_TARGET_NAME} matches to regex: ${ADD_TARGET_TEST_TARGET_EXCLUDE}, so the test target wouldn't be built.")
                return()
            endif ()
        endif ()
    endif ()

    if (ADD_TARGET_BENCHMARK_TARGET) # only benchmark targets can be excluded/include by match pattern via this script.
        if (ADD_TARGET_BENCHMARK_TARGET_INCLUDE)
            # check if include match pattern was defined and this target match to the pattern. If not match, don't create the target.
            if (NOT "${ADD_TARGET_TARGET_NAME}" MATCHES "${ADD_TARGET_BENCHMARK_TARGET_INCLUDE}")
                logStatus("Target ${ADD_TARGET_TARGET_NAME} doesn't match to regex: ${ADD_TARGET_BENCHMARK_TARGET_INCLUDE}, so the benchmark target wouldn't be built.")
                return()
            endif ()
        endif ()
        # check if exclude match pattern was defined and this target match to the pattern. If match, don't create the target.
        if (ADD_TARGET_BENCHMARK_TARGET_EXCLUDE)
            if ("${ADD_TARGET_TARGET_NAME}" MATCHES "${ADD_TARGET_BENCHMARK_TARGET_EXCLUDE}")
                logStatus("Target ${ADD_TARGET_TARGET_NAME} matches to regex: ${ADD_TARGET_BENCHMARK_TARGET_EXCLUDE}, so the benchmark target wouldn't be built.")
                return()
            endif ()
        endif ()
    endif ()

    # create groups for header files and collect all header (some IDEs like Visual Studio requires to have source file with header file in add_library/add_executable to have them visible).
    # public header file directories.
    addGroupsInternal(ROOT_PATH "${ADD_TARGET_TARGET_DIR}" 
        PATHS_TO_DIRS "${ADD_TARGET_PUBLIC_INC_DIRS}" 
        EXTENSIONS "hpp" "h"
        OUTPUT_FILES PUBLIC_INCS_TO_SRC) # found header files will be in PUBLIC_INCS_TO_SRC variable.
        
    # private header file directories.
    addGroupsInternal(ROOT_PATH "${ADD_TARGET_TARGET_DIR}" 
        PATHS_TO_DIRS "${ADD_TARGET_PRIVATE_INC_DIRS}" 
        EXTENSIONS "hpp" "h"
        OUTPUT_FILES PRIVATE_INCS_TO_SRC) # found header files will be in PRIVATE_INCS_TO_SRC variable.
   
    # create groups from source files.
    addSrcGroupsInternal(ROOT_PATH "${ADD_TARGET_TARGET_DIR}" 
        PATHS_TO_SRCS "${ALL_SRC}")
        
    set(SRC "${ALL_SRC}" "${PUBLIC_INCS_TO_SRC}" "${PRIVATE_INCS_TO_SRC}")
    
    if ("${ADD_TARGET_TARGET_TYPE}" STREQUAL "EXE")
        # create executable target.
        add_executable("${ADD_TARGET_TARGET_NAME}" "${SRC}")
        # set output directory for built target.
        if (NOT DEFINED ADD_TARGET_TARGET_OUTPUT_DIR)
            if (CMAKE_RUNTIME_OUTPUT_DIRECTORY)
                # assume PDB in the same folder as exec. Not used CMAKE_PDB_OUTPUT_DIRECTORY .
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            else()
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
            endif ()
        endif()
        set_target_properties("${ADD_TARGET_TARGET_NAME}" PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}"
            PDB_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}")
            
    elseif ("${ADD_TARGET_TARGET_TYPE}" STREQUAL "STATIC")
        # create static library target.
        add_library("${ADD_TARGET_TARGET_NAME}" STATIC "${SRC}")
        # set output directory for built target.
        if (NOT DEFINED ADD_TARGET_TARGET_OUTPUT_DIR)
            if (CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
            else()
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
            endif ()
        endif()
        set_target_properties("${ADD_TARGET_TARGET_NAME}" PROPERTIES 
            ARCHIVE_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}"
            PDB_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}")
        
    elseif ("${ADD_TARGET_TARGET_TYPE}" STREQUAL "SHARED")
        # create static library target.
        add_library("${ADD_TARGET_TARGET_NAME}" SHARED "${SRC}")
        # set output directory for built target.
        if (NOT DEFINED ADD_TARGET_TARGET_OUTPUT_DIR)
            if (CMAKE_RUNTIME_OUTPUT_DIRECTORY)
                # assume PDB in the same folder as exec. Not used CMAKE_PDB_OUTPUT_DIRECTORY 
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            else()
                set(ADD_TARGET_TARGET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
            endif ()
        endif()
        
        set_target_properties("${ADD_TARGET_TARGET_NAME}" PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}"
            PDB_OUTPUT_DIRECTORY "${ADD_TARGET_TARGET_OUTPUT_DIR}")
        
        if (MSVC AND ADD_TARGET_WIN_DLL_EXPORT_ALL_SYMBOLS)
            # Microsoft Visual Studio: if WIN_DLL_EXPORT_ALL_SYMBOLS was provided, all symbols from DLL will be visible. 
            set_target_properties("${ADD_TARGET_TARGET_NAME}" PROPERTIES 
                WINDOWS_EXPORT_ALL_SYMBOLS "1")
        endif ()
        
    elseif ("${ADD_TARGET_TARGET_TYPE}" STREQUAL "INTERFACE")
        # create interface library target.
        add_library("${ADD_TARGET_TARGET_NAME}" INTERFACE)
    else ()
        message(FATAL_ERROR " Incorrect TARGET_TYPE=${ADD_TARGET_TARGET_TYPE} for TARGET_NAME=${ADD_TARGET_TARGET_NAME} from location ${ADD_TARGET_TARGET_DIR}")
    endif ()
    if ("${ADD_TARGET_TARGET_TYPE}" STREQUAL "INTERFACE")
        # interface target has its own token INTERFACE for target commands.
        target_include_directories("${ADD_TARGET_TARGET_NAME}" INTERFACE "${ADD_TARGET_PUBLIC_INC_DIRS}")
        
        target_link_libraries("${ADD_TARGET_TARGET_NAME}" INTERFACE "${ADD_TARGET_PUBLIC_LINK_TARGETS}")
        
        target_compile_definitions("${ADD_TARGET_TARGET_NAME}" INTERFACE "${ADD_TARGET_PUBLIC_DEFINES}" "${GLOBAL_COMPILE_DEFINES}")
        
        target_link_options("${ADD_TARGET_TARGET_NAME}" INTERFACE "${ADD_TARGET_PUBLIC_LINK_OPTIONS}")

        target_compile_options("${ADD_TARGET_TARGET_NAME}" INTERFACE "${ADD_TARGET_PUBLIC_COMPILE_OPTIONS}")

		if(MSVC)
			target_compile_options("${ADD_TARGET_TARGET_NAME}" INTERFACE "/Zc:__cplusplus")
		endif()
    else()
        target_include_directories("${ADD_TARGET_TARGET_NAME}" PUBLIC "${ADD_TARGET_PUBLIC_INC_DIRS}")
        target_include_directories("${ADD_TARGET_TARGET_NAME}" PRIVATE "${ADD_TARGET_PRIVATE_INC_DIRS}")
        
        target_link_libraries("${ADD_TARGET_TARGET_NAME}" PUBLIC "${ADD_TARGET_PUBLIC_LINK_TARGETS}")
        target_link_libraries("${ADD_TARGET_TARGET_NAME}" PRIVATE "${ADD_TARGET_PRIVATE_LINK_TARGETS}")
        
        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
            set(ADD_TARGET_BUILD_TYPE "ADD_TARGET_RELEASE=1")
        else()
            set(ADD_TARGET_BUILD_TYPE "ADD_TARGET_DEBUG=1")
        endif()
        target_compile_definitions("${ADD_TARGET_TARGET_NAME}" PUBLIC "${ADD_TARGET_PUBLIC_DEFINES}" "${GLOBAL_COMPILE_DEFINES}" "${ADD_TARGET_BUILD_TYPE}")
        target_compile_definitions("${ADD_TARGET_TARGET_NAME}" PRIVATE "${ADD_TARGET_PRIVATE_DEFINES}" "${GLOBAL_COMPILE_DEFINES}")
        
        target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "${ADD_TARGET_PUBLIC_LINK_OPTIONS}")
        target_link_options("${ADD_TARGET_TARGET_NAME}" PRIVATE "${ADD_TARGET_PRIVATE_LINK_OPTIONS}")

        target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "${ADD_TARGET_PUBLIC_COMPILE_OPTIONS}" "${GLOBAL_INTERNAL_COMPILE_OPTIONS}")
        target_compile_options("${ADD_TARGET_TARGET_NAME}" PRIVATE "${ADD_TARGET_PRIVATE_COMPILE_OPTIONS}" "${GLOBAL_INTERNAL_COMPILE_OPTIONS}" )

		if(MSVC)
			target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "/Zc:__cplusplus")
		endif()
    endif()
    
    if (ADD_TARGET_TEST_TARGET)
        set(ADD_TARGET_TEST_COMMAND "${ADD_TARGET_TARGET_NAME}")
        
        # check if user provided a spectial function to define test and test command.
        if (COMMAND addTargetTestRunCommand)
            AddTargetTestRunCommand("${ADD_TARGET_TARGET_NAME}")
        else()
            if (ADD_TARGET_VALGRIND)
                # if tests shall be run with valgrind
                AddTargetTestRunCommand_Valgrind("${ADD_TARGET_TARGET_NAME}")
            endif ()

            if (ADD_TARGET_DR_MEMORY)
                # if tests shall be run with dr memory
                AddTargetTestRunCommand_DrMemory("${ADD_TARGET_TARGET_NAME}")
            endif ()
        endif ()
        if (ADD_TARGET_TEST_SHELL_COMMAND)
            add_test(NAME "${ADD_TARGET_TARGET_NAME}" COMMAND sh -c "${ADD_TARGET_TEST_COMMAND}" ${GTEST_ARGS_LIST} WORKING_DIRECTORY "$<TARGET_FILE_DIR:${ADD_TARGET_TARGET_NAME}>")
            
        else()
            add_test(NAME "${ADD_TARGET_TARGET_NAME}" 
                COMMAND  ${ADD_TARGET_TEST_COMMAND} ${GTEST_ARGS_LIST}
                WORKING_DIRECTORY "$<TARGET_FILE_DIR:${ADD_TARGET_TARGET_NAME}>"
                )
        endif()

        if (GCOV_ENABLE AND GLOBAL_INTERNAL_COMPILER_GCC)
            foreach(SRC_ITEM IN LISTS ADD_TARGET_SRC)
                set(GCOV_CONFIG "${GCOV_CONFIG}filter = ${SRC_ITEM}\n")
            endforeach()

            file(APPEND "${GCOV_CONF_PATH}" "${GCOV_CONFIG}")
        endif()
    endif()
    
    # gcov
    if ((NOT "${ADD_TARGET_TARGET_TYPE}" STREQUAL "INTERFACE") AND GCOV_ENABLE AND GLOBAL_INTERNAL_COMPILER_GCC)
        target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fprofile-arcs" "-ftest-coverage")
        target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "--coverage")
        target_link_libraries("${ADD_TARGET_TARGET_NAME}" PUBLIC "gcov")
    endif()

    # CONAN PART.
    # Download/build targets defined in conanfile.txt.
    set(CONAN_FILE_PATH "${ADD_TARGET_TARGET_DIR}/conanfile.txt")
    if (EXISTS "${CONAN_FILE_PATH}")
        file(RELATIVE_PATH CONAN_FILE_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_FILE_PATH}") # conan_cmake_run requires relative path

        # Note that build type, architecture is detected by CMake, but you can overwrite it
        # see https://github.com/conan-io/cmake-conan for details
        conan_cmake_run(CONANFILE "${CONAN_FILE_RELATIVE_PATH}"
                        BASIC_SETUP         # calls conan_basic_setup: https://docs.conan.io/en/latest/reference/generators/cmake.html
                        CMAKE_TARGETS       # to use modern CMake approach: target_* function
                        BUILD missing)      # build library if missing 
        logStatus("Libraries taken via Conan.io: ${CONAN_TARGETS}")
        target_link_libraries("${ADD_TARGET_TARGET_NAME}" PUBLIC  ${CONAN_TARGETS})
    endif()
    
    if (NOT "${ADD_TARGET_TARGET_TYPE}" STREQUAL "INTERFACE")
        # add compiler and linker flags required by sanitizers.

        if (ADD_TARGET_CLANG_ADDRESS_SANITIZER)
            target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=address" "-fno-omit-frame-pointer")
            target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=address" "-fno-omit-frame-pointer")
        endif ()
        if (ADD_TARGET_CLANG_MEMORY_SANITIZER)
            target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=memory" "-fsanitize-memory-track-origins" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
            target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=memory" "-fsanitize-memory-track-origins" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
        endif ()
        if (ADD_TARGET_CLANG_THREAD_SANITIZER)
            target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=thread" "-fno-omit-frame-pointer")
            target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=thread" "-fno-omit-frame-pointer")
        endif ()
        if (ADD_TARGET_CLANG_UNDEFINED_BEHAVIOR_SANITIZER)
            target_link_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=undefined")
            target_compile_options("${ADD_TARGET_TARGET_NAME}" PUBLIC "-fsanitize=undefined")
        endif ()
    endif ()

    # RESOURCE PART.
    # RES_COPY_TO_EXE_DIR will contain all resources, that shall be copied to executable location which links this target. Moreover it will contant all resources from target that are linked to this target - recoursive accumulation.
    set(RES_COPY_TO_EXE_DIR ${ADD_TARGET_RESOURCES_TO_COPY_TO_EXE_DIR})
    # similar case as RES_COPY_TO_EXE_DIR but all resources have destination point.
    set(RES_COPY ${ADD_TARGET_RESOURCES_TO_COPY})

    list(APPEND LIBS ${ADD_TARGET_PUBLIC_LINK_TARGETS} ${ADD_TARGET_PRIVATE_LINK_TARGETS})

    list(APPEND ALL_LIBS "${LIBS}")
    foreach(LIB IN LISTS LIBS)
        # collect resources from target that are linked to this target.
        list(APPEND RES_COPY_TO_EXE_DIR ${${LIB}_PROPERTY_RESOURCES_TO_COPY_TO_EXE_DIR})
        list(APPEND RES_COPY ${${LIB}_PROPERTY_RESOURCES_TO_COPY})
        # there is no CMake support to collect all libraries linked to given target, so it would be done by definition of ${TARGET_NAME}_PROPERTY_ALL_LIBS.
        # This variable will be used to copy all DLL to executable file location.
        list(APPEND ALL_LIBS ${${LIB}_PROPERTY_ALL_LIBS})
    endforeach()

    # define global variables with resources information and target dir. It cannot be done by properties, because interface target doesn't allow user defined properties.
    set(${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY_TO_EXE_DIR "${RES_COPY_TO_EXE_DIR}" CACHE INTERNAL "Resource of ${ADD_TARGET_TARGET_NAME} to copy to exe dir" FORCE)
    set(${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY "${RES_COPY}" CACHE INTERNAL "Resource of ${ADD_TARGET_TARGET_NAME} to copy to given dir" FORCE)
    set(${ADD_TARGET_TARGET_NAME}_PROPERTY_TARGET_DIR "${ADD_TARGET_TARGET_DIR}" CACHE INTERNAL "Path where target ${ADD_TARGET_TARGET_NAME} is defined." FORCE)
    set(${ADD_TARGET_TARGET_NAME}_PROPERTY_ALL_LIBS "${ALL_LIBS}" CACHE INTERNAL "All libs (transitive) linked to build target ${ADD_TARGET_TARGET_NAME}." FORCE)

    if("${ADD_TARGET_TARGET_TYPE}" STREQUAL "EXE")
        # copy resources only if this target is executable target.
        
        foreach(RES IN LISTS ${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY_TO_EXE_DIR)

            if (NOT EXISTS "${RES}")
                logError("Resource ${RES} not exists.")
            else()
                if (IS_DIRECTORY "${RES}")
                    get_filename_component(DIR_NAME "${RES}" NAME_WE)
                    add_custom_command(TARGET ${ADD_TARGET_TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                            "${RES}" 
                            "$<TARGET_FILE_DIR:${ADD_TARGET_TARGET_NAME}>/${DIR_NAME}")  # copy_directory copies content without folder, so folder needs to be added to destination path.
                else ()
                    add_custom_command(TARGET ${ADD_TARGET_TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                            "${RES}" 
                            "$<TARGET_FILE_DIR:${ADD_TARGET_TARGET_NAME}>")  

                endif ()
            endif ()
        endforeach()

        if (${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY)
            list(LENGTH ${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY RES_LIST_LENGTH)

            math(EXPR CHECK_LENGTH "${RES_LIST_LENGTH}%3")
            if (NOT "${CHECK_LENGTH}" STREQUAL "0")
                logError("Incorrect format of RESOURCES_TO_COPY values. Expected \"src TO dest\".")
            endif ()

            foreach (KEYWORD_INDEX RANGE 1 ${RES_LIST_LENGTH} 3) # RANGE start stop step

                math(EXPR INDEX "${KEYWORD_INDEX}-1")
                list(GET ${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${INDEX} SRC)

                list(GET ${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${KEYWORD_INDEX} KEYWORD)

                math(EXPR DEST_INDEX "${KEYWORD_INDEX}+1")
                list(GET ${ADD_TARGET_TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${DEST_INDEX} DEST)

                # check if the format is correct: src_path1 TO dest_path1; src_path2 TO dest_path2 (etc.) 
                if (NOT "${KEYWORD}" STREQUAL "TO")
                    logError("Incorrect key for RESOURCES_TO_COPY in target: ${ADD_TARGET_TARGET_NAME} after item: ${SRC}")
                endif()
                if (IS_DIRECTORY "${SRC}")
                    get_filename_component(DIR_NAME "${SRC}" NAME_WE)
                    add_custom_command(TARGET ${ADD_TARGET_TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST}/${DIR_NAME}"
                        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                            "${SRC}" 
                            "${DEST}/${DIR_NAME}")  
                else ()
                    add_custom_command(TARGET ${ADD_TARGET_TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST}"
                        COMMAND ${CMAKE_COMMAND} -E copy
                            "${SRC}" 
                            "${DEST}")  
                endif ()

            endforeach ()
        endif()

        # copy dynanic libs 
        foreach(LIB IN LISTS ${ADD_TARGET_TARGET_NAME}_PROPERTY_ALL_LIBS)
            get_target_property(OUTPUT_TYPE ${LIB} TYPE)
            if ("${OUTPUT_TYPE}" STREQUAL "SHARED_LIBRARY")
                add_custom_command(TARGET ${ADD_TARGET_TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                        "$<TARGET_FILE:${LIB}>" 
                        "$<TARGET_FILE_DIR:${ADD_TARGET_TARGET_NAME}>")  
            endif()
        endforeach()
    endif()

    # benchmark
    if (ADD_TARGET_BENCHMARK_TARGET)

        add_custom_command(TARGET "${ADD_TARGET_TARGET_NAME}" POST_BUILD 
            COMMAND  ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:${ADD_TARGET_TARGET_NAME}>" >> "${CMAKE_BINARY_DIR}/paths_to_benchmarks.txt" 
        )

    endif()

endfunction()

# \brief Prints information about the given target name.
function(printTarget TARGET_NAME)
    logStatus("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")

    if (NOT TARGET ${TARGET_NAME})
        logStatus("NO TARGET NAMED: ${TARGET_NAME}")
        return()
    endif()

    logStatus("TARGET NAME: ${TARGET_NAME}")
    get_target_property(OUTPUT_TYPE ${TARGET_NAME} TYPE)
    logStatus("TARGET TYPE: ${OUTPUT_TYPE}")
    logStatus("ROOT: ${${TARGET_NAME}_PROPERTY_TARGET_DIR}")

    get_target_property(OUTPUT_SOURCES ${TARGET_NAME} SOURCES)
    logListStatus(OUTPUT_SOURCES "Source files: " "SRC: ")
        
    get_target_property(OUTPUT_INT_INC_DIR ${TARGET_NAME} INTERFACE_INCLUDE_DIRECTORIES)
    logListStatus(OUTPUT_INT_INC_DIR "Public include dirs: " "DIR: ")

    get_target_property(OUTPUT_INC_DIR ${TARGET_NAME} INCLUDE_DIRECTORIES)
    logListStatus(OUTPUT_INC_DIR "Include dirs: " "DIR: ")
    
    get_target_property(OUTPUT_INT_COMPILE_DEF ${TARGET_NAME} INTERFACE_COMPILE_DEFINITIONS)
    logListStatus(OUTPUT_INT_COMPILE_DEF "Public defines: " "DEF: ")
    
    get_target_property(OUTPUT_COMPILE_DEF ${TARGET_NAME} COMPILE_DEFINITIONS)
    logListStatus(OUTPUT_COMPILE_DEF "Defines: " "DEF: ")
    
    get_target_property(OUTPUT_INT_COMPILE_OPT ${TARGET_NAME} INTERFACE_COMPILE_OPTIONS)
    logListStatus(OUTPUT_INT_COMPILE_OPT "Public compile flags: " "FLAG: ")
 
    get_target_property(OUTPUT_COMPILE_OPT ${TARGET_NAME} COMPILE_OPTIONS)
    logListStatus(OUTPUT_COMPILE_OPT "Compile flags: " "FLAG: ")
    
    get_target_property(OUTPUT_INT_LINK_OPT ${TARGET_NAME} INTERFACE_LINK_OPTIONS)
    logListStatus(OUTPUT_INT_LINK_OPT "Public link flags: " "FLAG: ")
    
    get_target_property(OUTPUT_LINK_OPT ${TARGET_NAME} LINK_OPTIONS)
    logListStatus(OUTPUT_LINK_OPT "Link flags: " "FLAG: ")
    
    logListStatus(${TARGET_NAME}_PROPERTY_RESOURCES "Resources that will be copied to each executable file location that links this target or just this target resources:" "RES: ")
    
    if (${TARGET_NAME}_PROPERTY_TARGET_DIR)
    
        list(LENGTH ${TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY RES_LIST_LENGTH)
        logStatus("Resources that will be copied to each executable file location that links this target or just this target resources:")
        
        foreach (KEYWORD_INDEX RANGE 1 ${RES_LIST_LENGTH} 3) # RANGE start stop step

            math(EXPR INDEX "${KEYWORD_INDEX}-1")
            list(GET ${TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${INDEX} SRC)

            list(GET ${TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${KEYWORD_INDEX} KEYWORD)

            math(EXPR DEST_INDEX "${KEYWORD_INDEX}+1")
            list(GET ${TARGET_NAME}_PROPERTY_RESOURCES_TO_COPY ${DEST_INDEX} DEST)

            logStatus("Res: ${SRC} TO ${DEST}")
            
        endforeach ()
    endif ()
    
    logListStatus(${TARGET_NAME}_PROPERTY_ALL_LIBS "Transivite link targets: " "TARGET: ")
    
    logStatus("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
endfunction()

# \brief Creates target. See comment of AddTargetInternal.
macro(AddTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" ${ARGV})
    
endmacro()

# \brief Creates target. See comment of AddTargetInternal.
macro(AddExeTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "EXE" ${ARGV})
    
endmacro()

# \brief Creates target. See comment of AddTargetInternal.
macro(AddStaticLibTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "STATIC" ${ARGV})
    
endmacro()


# \brief Creates target. See comment of AddTargetInternal.
macro(AddSharedLibTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "SHARED" ${ARGV})
    
endmacro()


macro(AddInterfaceTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "INTERFACE" ${ARGV})
    
endmacro()



# \brief Creates target. See comment of AddTargetInternal.
macro(AddTestTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "EXE"  ${ARGV} TEST_TARGET)
    
endmacro()

# \brief Creates target. See comment of AddTargetInternal.
macro(AddBenchmarkTarget)

    AddTargetInternal(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}" TARGET_LINE "${CMAKE_CURRENT_LIST_LINE}" TARGET_TYPE "EXE" ${ARGV} BENCHMARK_TARGET)
    
endmacro()
