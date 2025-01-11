
function(check_and_clone_library library_name github_repo clone_dir)
    find_package(${library_name} QUIET)

    if (NOT ${library_name}_FOUND)
        message(STATUS "${library_name} is not installed on the system.")

        # Check if the library is already cloned
        if (NOT EXISTS "${clone_dir}")
            message(STATUS "Cloning ${library_name} from ${github_repo}...")

            execute_process(
                COMMAND git clone --depth 1 ${github_repo} ${clone_dir}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE GIT_RESULT
            )

            if (NOT GIT_RESULT EQUAL 0)
                message(FATAL_ERROR "Failed to clone ${library_name} from ${github_repo}.")
            endif()
        else()
            message(STATUS "${library_name} is already cloned at ${clone_dir}.")
        endif()

        # Add the cloned directory to the build
        add_subdirectory(${clone_dir})
    else()
        message(STATUS "${library_name} is already installed on the system.")
    endif()
endfunction()

