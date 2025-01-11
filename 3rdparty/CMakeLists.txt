

# NOTE: Options for submodules
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "Disable Benchmark testing")


# NOTE: If the library is not installed will clone it in the 3rdparty folder
set(SPDLOG_GITHUB_REPO "https://github.com/gabime/spdlog.git")
set(SPDLOG_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spdlog")
check_and_clone_library(spdlog ${SPDLOG_GITHUB_REPO} ${SPDLOG_SUBMODULE_DIR})


set(FMT_GITHUB_REPO "https://github.com/fmtlib/fmt.git")
set(FMT_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fmt")
check_and_clone_library(fmt ${FMT_GITHUB_REPO} ${FMT_SUBMODULE_DIR})



set(GLFW_GITHUB_REPO "https://github.com/glfw/glfw.git")
set(GLFW_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw")
check_and_clone_library(glfw ${GLFW_GITHUB_REPO} ${GLFW_SUBMODULE_DIR})



set(SHADERC_SKIP_TESTS ON)
set(SHADERC_GITHUB_REPO "https://github.com/google/shaderc.git")
set(SHADERC_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaderc")

if(NOT EXISTS "${SHADERC_SUBMODULE_DIR}")
    message(STATUS "Cloning Shaderc from ${SHADERC_GITHUB_REPO}")
    execute_process(
        COMMAND git clone --recurse-submodules ${SHADERC_GITHUB_REPO} ${SHADERC_SUBMODULE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_CLONE_RESULT
    )
    if(NOT GIT_CLONE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone Shaderc repository. Git returned ${GIT_CLONE_RESULT}.")
    endif()
else()
    message(STATUS "Shaderc directory already exists: ${SHADERC_SUBMODULE_DIR}")
endif()

# Add just this simple check after shaderc clone
if(EXISTS ${SHADERC_SUBMODULE_DIR} AND NOT EXISTS "${SHADERC_SUBMODULE_DIR}/third_party/spirv-tools/")
    execute_process(
        COMMAND ${SHADERC_SUBMODULE_DIR}/utils/git-sync-deps
        message("=========== running the command at path ${SHADERC_SUBMODULE_DIR}/utils/git-sync-deps  =============")
    )
endif()


# NOTE: This code is generated, there is not a github repo
add_subdirectory(glad)
