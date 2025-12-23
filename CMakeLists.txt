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
    src/audio.cpp
    src/texture.cpp
    src/SpriteManager.cpp
    src/model.cpp
    src/ui.cpp
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
    src/audio.h
    src/texture.h
    src/SpriteManager.h
    src/SpriteData.h
    src/model.h
    src/ui.h
    src/miniaudio.h
    src/stb_image.h
    src/tiny_gltf.h
    glad/glad.h
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
endif()

add_custom_command(TARGET voxel_pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:voxel_pacman>/shaders
)

add_custom_command(TARGET voxel_pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/levels $<TARGET_FILE_DIR:voxel_pacman>/levels
)

add_custom_command(TARGET voxel_pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:voxel_pacman>/assets
)

if(MSVC)
    target_compile_options(voxel_pacman PRIVATE /W3 /bigobj)
endif()
