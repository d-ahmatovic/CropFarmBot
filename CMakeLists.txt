cmake_minimum_required(VERSION 3.10)

project(FarmBot)

# Configure Botcraft
set(BOTCRAFT_GAME_VERSION 1.21.8)
set(BOTCRAFT_BUILD_EXAMPLES OFF)
set(BOTCRAFT_BUILD_TESTS OFF)
set(BOTCRAFT_BUILD_TESTS_ONLINE OFF)
set(BOTCRAFT_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(BOTCRAFT_COMPRESSION ON)
set(BOTCRAFT_ENCRYPTION ON)
set(BOTCRAFT_USE_OPENGL_GUI OFF)
set(BOTCRAFT_USE_IMGUI OFF)
set(BOTCRAFT_WINDOWS_BETTER_SLEEP ON)
set(BOTCRAFT_USE_PRECOMPILED_HEADERS ON)
set(BOTCRAFT_BUILD_DOC OFF)

# Configure nlohmann_json
set(JSON_BuildTests OFF)

# Botcraft is the bot library used to deliver functionality
add_subdirectory(dependencies/Botcraft)
add_subdirectory(dependencies/nlohmann_json)

# Here's the main code and application located
add_subdirectory(Sources)
