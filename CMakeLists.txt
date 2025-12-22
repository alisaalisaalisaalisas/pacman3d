cmake_minimum_required(VERSION 3.16)
project(VoxelPacman VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(SOURCES
    src/main.cpp
    src/shader.cpp
    src/mesh.cpp
    src/camera.cpp
    src/maze.cpp
    src/renderer.cpp
    src/entity.cpp
    src/pacman.cpp
    src/ghost.cpp
    glad/glad.c
)

set(HEADERS
    src/shader.h
    src/mesh.h
    src/camera.h
    src/maze.h
    src/renderer.h
    src/entity.h
    src/pacman.h
    src/ghost.h
    glad/glad.h
    glad/khrplatform.h
)

add_executable(voxel_pacman ${SOURCES} ${HEADERS})

target_include_directories(voxel_pacman PRIVATE
    ${CMAKE_SOURCE_DIR}/glad
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(voxel_pacman PRIVATE
    OpenGL::GL
    glfw
    glm::glm
)

if(WIN32)
    target_link_libraries(voxel_pacman PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(voxel_pacman PRIVATE dl)
endif()

add_custom_command(TARGET voxel_pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:voxel_pacman>/shaders
)

add_custom_command(TARGET voxel_pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/levels $<TARGET_FILE_DIR:voxel_pacman>/levels
)

if(MSVC)
    target_compile_options(voxel_pacman PRIVATE /W4)
else()
    target_compile_options(voxel_pacman PRIVATE -Wall -Wextra -Wpedantic)
endif()
