#-------------------------------------------------------------------------------
#  FORGE CONFIGURATION
#-------------------------------------------------------------------------------
project(Forge LANGUAGES CXX)
set(TARGET ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Add Forge source files
#-------------------------------------------------------------------------------
file(GLOB_RECURSE EXEC_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.h"
)
file(GLOB_RECURSE EXEC_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.cpp"
)

add_library(${TARGET}  
    ${EXEC_HEADERS}
    ${EXEC_SRC}
)

# Link dependencies
target_link_libraries(${TARGET} PUBLIC 
    spdlog 
    fmt         
    glfw
    shaderc_combined
    glslang
    SPIRV-Tools
    SPIRV-Tools-opt
)

# Conditional dependencies
if(RESHAPE_ADD_SUPPORT_OPENGL)
    target_link_libraries(${TARGET} PUBLIC glad)
endif()

if(RESHAPE_ADD_SUPPORT_VULKAN)
    target_link_libraries(${TARGET} PUBLIC 
        vulkan
    )
endif()

#-------------------------------------------------------------------------------
# Profiling Configuration (Optional)
#-------------------------------------------------------------------------------
if(RESHAPE_ADD_SUPPORT_PROFILING)
    add_compile_definitions(RESHAPE_PROFILING_ENABLED)
    find_package(Tracy REQUIRED)

    if (NOT Tracy_FOUND)
        message(FATAL_ERROR "Tracy profiling tool not found. Continuing without linking.")
    else()
        message(STATUS "Profiling enabled, Tracy library linked successfully.")
        add_compile_definitions(TRACY_BUILD_AND_LINKED)  
        target_link_libraries(${TARGET} PRIVATE TracyClient)
        target_include_directories(${TARGET} PUBLIC
            /usr/local/include/tracy
        )
    endif()
endif()

# Include directories for the target
target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)


#-------------------------------------------------------------------------------
# Install Configuration
#-------------------------------------------------------------------------------
install(TARGETS ${TARGET}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Export the headers
set_target_properties(${TARGET} PROPERTIES
    PUBLIC_HEADER "${EXEC_HEADERS}"
)
