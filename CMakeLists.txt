cmake_minimum_required(VERSION 3.15)
project(DnD_DM_Toolbox VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)

# Dear ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)

# nlohmann/json (header-only)
set(JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/json/include)

# nativefiledialog-extended (file dialogs)
# PIC required because nfd is also linked into shared plugin libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(libs/nfd)

# Main executable
file(GLOB_RECURSE SOURCES
    src/core/*.cpp
    src/ui/*.cpp
)

file(GLOB_RECURSE HEADERS
    include/*.h
    include/**/*.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} src/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${JSON_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    OpenGL::GL
    glfw
    SQLite::SQLite3
    nfd
    ${CMAKE_DL_LIBS}
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# ============================================================
# PLUGINS (Shared Libraries)
# ============================================================

function(add_plugin PLUGIN_NAME)
    set(PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/${PLUGIN_NAME})

    file(GLOB_RECURSE PLUGIN_SOURCES ${PLUGIN_DIR}/*.cpp)

    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})

    target_include_directories(${PLUGIN_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${JSON_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
    )

    target_link_libraries(${PLUGIN_NAME} PRIVATE
        SQLite::SQLite3
        imgui
    )

    set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugins
    )

    add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PLUGIN_DIR}/${PLUGIN_NAME}.manifest.json
            ${CMAKE_CURRENT_BINARY_DIR}/plugins/${PLUGIN_NAME}.manifest.json
    )

    # Copier les éventuels schémas/seeds SQL du plugin (chargés à l'exécution)
    file(GLOB_RECURSE PLUGIN_SQL_FILES ${PLUGIN_DIR}/*.sql)
    foreach(SQL_FILE ${PLUGIN_SQL_FILES})
        get_filename_component(SQL_FILENAME ${SQL_FILE} NAME)
        add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${SQL_FILE}
                ${CMAKE_CURRENT_BINARY_DIR}/plugins/${SQL_FILENAME}
        )
    endforeach()
endfunction()

# Compiler les plugins
add_plugin(note)
add_plugin(dice_roller)
add_plugin(log_viewer)
add_plugin(race_dd_5_5)
add_plugin(debug_monitor)
add_plugin(classe_manager)
target_link_libraries(classe_manager PRIVATE nfd)
