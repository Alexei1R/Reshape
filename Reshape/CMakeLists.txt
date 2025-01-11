
#-------------------------------------------------------------------------------
#  RESHAPE CONFIGURATION
#-------------------------------------------------------------------------------
project(Reshape LANGUAGES CXX)
set(TARGET ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Add Reshape source files
#-------------------------------------------------------------------------------
file(GLOB_RECURSE EXEC_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
file(GLOB_RECURSE EXEC_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${TARGET}  
    ${EXEC_HEADERS}
    ${EXEC_SRC}
)

# Link dependencies
target_link_libraries(${TARGET} PRIVATE 
    Forge
)




# Include directories for the target
target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# Set target properties (versioning)
set_target_properties(${TARGET} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)

# Set the C++ standard for the TARGET
target_compile_features(${TARGET} PRIVATE cxx_std_23)

#-------------------------------------------------------------------------------
# Install Configuration
#-------------------------------------------------------------------------------
install(TARGETS ${TARGET}
    RUNTIME DESTINATION bin
)
