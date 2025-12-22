/**
 * Voxel Pac-Man 3D - Milestone 6: Collision & Game Over
 */

#include <iostream>
#include <cmath>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "maze.h"
#include "renderer.h"
#include "pacman.h"
#include "ghost.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

constexpr float SKY_R = 128.0f / 255.0f;
constexpr float SKY_G = 180.0f / 255.0f;
constexpr float SKY_B = 230.0f / 255.0f;

float g_camera_angle = 45.0f;
float g_camera_distance = 25.0f;
float g_camera_height = 20.0f;
Direction g_input_dir = Direction::NONE;

bool checkCollision(const PacMan& pacman, const Ghost& ghost) {
    return (pacman.grid_x == ghost.grid_x && pacman.grid_y == ghost.grid_y);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_W: case GLFW_KEY_UP:    g_input_dir = Direction::DOWN; break;
            case GLFW_KEY_S: case GLFW_KEY_DOWN:  g_input_dir = Direction::UP; break;
            case GLFW_KEY_A: case GLFW_KEY_LEFT:  g_input_dir = Direction::LEFT; break;
            case GLFW_KEY_D: case GLFW_KEY_RIGHT: g_input_dir = Direction::RIGHT; break;
            case GLFW_KEY_Q: g_camera_angle -= 5.0f; break;
            case GLFW_KEY_E: g_camera_angle += 5.0f; break;
            case GLFW_KEY_R: g_camera_distance = std::max(10.0f, g_camera_distance - 2.0f); break;
            case GLFW_KEY_F: g_camera_distance = std::min(50.0f, g_camera_distance + 2.0f); break;
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwSetErrorCallback([](int e, const char* d) { std::cerr << "GLFW " << e << ": " << d << "\n"; });
    if (!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Voxel Pac-Man 3D", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);
    
    if (!gladLoadGL()) { glfwDestroyWindow(window); glfwTerminate(); return -1; }
    
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    Shader shader;
    if (!shader.load("shaders/vertex.glsl", "shaders/fragment.glsl")) return -1;
    
    Maze maze;
    if (!maze.load("levels/level1.txt")) return -1;
    
    MazeRenderer mazeRenderer;
    mazeRenderer.buildFromMaze(maze);
    
    PacMan pacman;
    pacman.createMesh();
    pacman.setGridPosition(14, 6, maze);
    
    std::vector<Ghost> ghosts;
    ghosts.emplace_back(GhostType::BLINKY);
    ghosts.emplace_back(GhostType::PINKY);
    ghosts.emplace_back(GhostType::INKY);
    ghosts.emplace_back(GhostType::CLYDE);
    
    int ghost_spawns[][2] = {{1, 23}, {26, 23}, {1, 1}, {26, 1}};
    for (int i = 0; i < 4; i++) {
        ghosts[i].createMesh();
        ghosts[i].respawn(maze, ghost_spawns[i][0], ghost_spawns[i][1]);
    }
    
    Camera camera;
    camera.setPerspective(45.0f, static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 100.0f);
    
    double prev_time = glfwGetTime();
    bool gameOver = false;
    float deathTimer = 0.0f;
    int ghostEatBonus = 200;
    
    std::cout << "\nLives: 3 | Score: 0 | Collect pellets, avoid ghosts!" << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - prev_time);
        prev_time = current_time;
        
        if (!gameOver && !pacman.isDead) {
            if (g_input_dir != Direction::NONE) {
                pacman.handleInput(g_input_dir, maze);
                g_input_dir = Direction::NONE;
            }
            pacman.continueMovement(maze);
            pacman.update(dt);
            
            // Collect pellets
            if (pacman.collectPellet(maze)) {
                // Power pellet - frighten ghosts
                ghostEatBonus = 200;
                for (auto& ghost : ghosts) {
                    ghost.setFrightened(8.0f);
                }
            }
            
            // Update ghosts
            glm::ivec2 ppos(pacman.grid_x, pacman.grid_y);
            for (auto& ghost : ghosts) {
                ghost.updateAI(maze, ppos);
                ghost.update(dt);
                
                // Check collision
                if (!ghost.isEaten && checkCollision(pacman, ghost)) {
                    if (ghost.mode == GhostMode::FRIGHTENED) {
                        ghost.isEaten = true;
                        pacman.score += ghostEatBonus;
                        std::cout << "Ate ghost! +" << ghostEatBonus << " pts" << std::endl;
                        ghostEatBonus *= 2;
                    } else {
                        pacman.die();
                        deathTimer = 1.5f;
                    }
                }
            }
        }
        else if (pacman.isDead) {
            deathTimer -= dt;
            if (deathTimer <= 0.0f) {
                if (pacman.lives <= 0) {
                    gameOver = true;
                    std::cout << "\n=== GAME OVER === Final Score: " << pacman.score << std::endl;
                } else {
                    pacman.respawn(maze);
                    for (int i = 0; i < 4; i++) {
                        ghosts[i].respawn(maze, ghost_spawns[i][0], ghost_spawns[i][1]);
                    }
                    std::cout << "Lives: " << pacman.lives << " | Score: " << pacman.score << std::endl;
                }
            }
        }
        
        camera.setupIsometric(g_camera_distance, g_camera_height, g_camera_angle);
        camera.target = maze.getCenter();
        
        glClearColor(SKY_R, SKY_G, SKY_B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        mazeRenderer.render(shader, camera);
        mazeRenderer.renderPellets(shader, maze);
        
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());
        
        if (!gameOver) {
            pacman.render(shader);
            for (auto& ghost : ghosts) ghost.render(shader);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    std::cout << "\nFinal Score: " << pacman.score << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
