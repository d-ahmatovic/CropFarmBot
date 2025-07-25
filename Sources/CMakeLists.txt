project(CropFarmer)

set(SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/CropFarmTasks.cpp
    ${PROJECT_SOURCE_DIR}/src/Config.cpp
)

set(HDR_FILES
    ${PROJECT_SOURCE_DIR}/includes/CropFarmTasks.hpp
    ${PROJECT_SOURCE_DIR}/includes/Config.hpp
)

# Create my target
add_executable(${PROJECT_NAME} ${HDR_FILES} ${SRC_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Add my include folder to the include path
target_include_directories(${PROJECT_NAME} PUBLIC includes)

# Linking my code to botcraft, cmake will handle everything
target_link_libraries(${PROJECT_NAME} botcraft nlohmann_json::nlohmann_json)

# All this stuff is not really necessary, we just set the output folder
# to be the same than for botcraft so we don't need to manually copy the shared
# library next to the executable
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_rd")
if(MSVC)
    # To avoid having one subfolder per configuration when building with Visual
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BOTCRAFT_OUTPUT_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BOTCRAFT_OUTPUT_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${BOTCRAFT_OUTPUT_DIR}/bin")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${BOTCRAFT_OUTPUT_DIR}/bin")
    
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${BOTCRAFT_OUTPUT_DIR}/bin")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BOTCRAFT_OUTPUT_DIR}/bin")
endif(MSVC)
