/**
 * Voxel Pac-Man 3D - Fixed Rotation + Eating Animation
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
#include "audio.h"
#include "model.h"
#include "ui.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

constexpr float SKY_R = 128.0f / 255.0f;
constexpr float SKY_G = 180.0f / 255.0f;
constexpr float SKY_B = 230.0f / 255.0f;

float g_camera_angle = 45.0f;
float g_camera_distance = 25.0f;
float g_camera_height = 20.0f;
Direction g_input_dir = Direction::NONE;

AudioManager* g_audio = nullptr;
UIManager* g_ui = nullptr;
double g_mouseX = 0, g_mouseY = 0;

bool checkCollision(const PacMan& pacman, const Ghost& ghost) {
    return (pacman.grid_x == ghost.grid_x && pacman.grid_y == ghost.grid_y);
}

float directionToAngle(Direction dir, bool reverse = false) {
    float base = 0.0f;
    switch (dir) {
        case Direction::UP:    base = 180.0f; break;
        case Direction::RIGHT: base = 270.0f; break;
        case Direction::DOWN:  base = 0.0f; break;
        case Direction::LEFT:  base = 90.0f; break;
        default: base = 0.0f;
    }
    return reverse ? base + 180.0f : base;
}

glm::vec3 getGhostTint(GhostType type) {
    switch (type) {
        case GhostType::BLINKY: return glm::vec3(1.0f, 0.2f, 0.2f);
        case GhostType::PINKY:  return glm::vec3(1.0f, 0.6f, 0.8f);
        case GhostType::INKY:   return glm::vec3(0.2f, 0.8f, 1.0f);
        case GhostType::CLYDE:  return glm::vec3(1.0f, 0.6f, 0.2f);
        default: return glm::vec3(1.0f);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Handle UI navigation with Enter/Escape
        if (g_ui && g_ui->getState() != GameState::PLAYING) {
            if (key == GLFW_KEY_ESCAPE && g_ui->getState() == GameState::PAUSED) {
                g_ui->hide();
                return;
            }
            if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) {
                g_ui->handleClick((float)g_mouseX, (float)g_mouseY);
                return;
            }
        }
        
        switch (key) {
            case GLFW_KEY_W: case GLFW_KEY_UP:    g_input_dir = Direction::UP; break;
            case GLFW_KEY_S: case GLFW_KEY_DOWN:  g_input_dir = Direction::DOWN; break;
            case GLFW_KEY_A: case GLFW_KEY_LEFT:  g_input_dir = Direction::RIGHT; break;
            case GLFW_KEY_D: case GLFW_KEY_RIGHT: g_input_dir = Direction::LEFT; break;
            case GLFW_KEY_Q: g_camera_angle -= 5.0f; break;
            case GLFW_KEY_E: g_camera_angle += 5.0f; break;
            case GLFW_KEY_R: g_camera_distance = std::max(10.0f, g_camera_distance - 2.0f); break;
            case GLFW_KEY_F: g_camera_distance = std::min(50.0f, g_camera_distance + 2.0f); break;
            case GLFW_KEY_M: if (g_audio) g_audio->stopMusic(); break;
            case GLFW_KEY_P: 
                if (g_ui && g_ui->getState() == GameState::PLAYING) g_ui->showPauseMenu();
                else if (g_ui && g_ui->getState() == GameState::PAUSED) g_ui->hide();
                break;
            case GLFW_KEY_ESCAPE: 
                if (g_ui && g_ui->getState() == GameState::PLAYING) g_ui->showPauseMenu();
                else if (g_ui && g_ui->getState() == GameState::MAIN_MENU) glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (g_ui) {
            g_ui->handleClick((float)g_mouseX, (float)g_mouseY);
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double x, double y) {
    g_mouseX = x;
    g_mouseY = y;
    if (g_ui) {
        g_ui->update((float)x, (float)y);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (g_ui) g_ui->setScreenSize(width, height);
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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(0); // Disable vsync for max FPS
    
    if (!gladLoadGL()) { glfwDestroyWindow(window); glfwTerminate(); return -1; }
    
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    AudioManager audio;
    g_audio = &audio;
    if (audio.init()) {
        audio.playMusic("assets/audio/music.wav");
    }
    
    Shader shader;
    if (!shader.load("shaders/vertex.glsl", "shaders/fragment.glsl")) return -1;
    
    Model pacmanModel, ghostModel, treeModel;
    bool usePacmanModel = pacmanModel.load("assets/sprites/PacmanFinal.glb");
    bool useGhostModel = ghostModel.load("assets/sprites/Ghosts.glb");
    bool useTreeModel = treeModel.load("assets/sprites/voxel trees 3d model.glb");
    
    Maze maze;
    if (!maze.load("levels/level1.txt")) return -1;
    
    MazeRenderer mazeRenderer;
    mazeRenderer.loadTextures();
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
    camera.setPerspective(45.0f, static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 200.0f);
    
    // Generate tree positions around maze perimeter (fewer for performance)
    std::vector<glm::vec3> treePositions;
    glm::vec3 mazeCenter = maze.getCenter();
    for (int i = 0; i < 12; i++) {
        float angle = (i / 12.0f) * 6.28f;
        float dist = 20.0f + (i % 2) * 4.0f;
        float x = mazeCenter.x + std::cos(angle) * dist;
        float z = mazeCenter.z + std::sin(angle) * dist;
        treePositions.push_back(glm::vec3(x, -0.5f, z));
    }
    
    // Cloud positions (fewer, larger)
    std::vector<glm::vec3> cloudPositions = {
        {mazeCenter.x - 12.0f, 15.0f, mazeCenter.z - 8.0f},
        {mazeCenter.x + 15.0f, 17.0f, mazeCenter.z + 5.0f},
        {mazeCenter.x, 20.0f, mazeCenter.z - 18.0f},
    };
    Mesh cloudMesh = createCube(glm::vec3(1.0f, 1.0f, 1.0f)); // White clouds
    
    // Voxel grass - larger cubes for better FPS
    Mesh grassMesh = createCube(glm::vec3(0.35f, 0.65f, 0.25f)); // Green grass cube
    Mesh dirtMesh = createCube(glm::vec3(0.55f, 0.4f, 0.25f)); // Brown dirt
    std::vector<glm::vec3> grassPositions;
    // Create grass grid - larger step for performance
    for (float x = mazeCenter.x - 45; x < mazeCenter.x + 45; x += 2.0f) {
        for (float z = mazeCenter.z - 45; z < mazeCenter.z + 45; z += 2.0f) {
            grassPositions.push_back(glm::vec3(x, -0.5f, z));
        }
    }
    
    double prev_time = glfwGetTime();
    bool gameOver = false;
    bool gameOverPrinted = false;
    float deathTimer = 0.0f;
    int ghostEatBonus = 200;
    int lastScore = 0;
    float eatAnimTime = 0.0f;
    
    // Initialize UI
    UIManager ui;
    ui.init();
    ui.setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    g_ui = &ui;
    
    // UI Callbacks
    ui.onStartGame = [&]() {
        ui.hide();
        gameOver = false;
        gameOverPrinted = false;
        pacman.lives = 3;
        pacman.score = 0;
        maze.load("levels/level1.txt");
        mazeRenderer.buildFromMaze(maze);
        pacman.respawn(maze);
        for (int i = 0; i < 4; i++)
            ghosts[i].respawn(maze, ghost_spawns[i][0], ghost_spawns[i][1]);
        audio.playMusic("assets/audio/music.wav");
    };
    
    ui.onResumeGame = [&]() {
        ui.hide();
    };
    
    ui.onRestartGame = [&]() {
        ui.hide();
        gameOver = false;
        gameOverPrinted = false;
        pacman.lives = 3;
        pacman.score = 0;
        maze.load("levels/level1.txt");
        mazeRenderer.buildFromMaze(maze);
        pacman.respawn(maze);
        for (int i = 0; i < 4; i++)
            ghosts[i].respawn(maze, ghost_spawns[i][0], ghost_spawns[i][1]);
        audio.playMusic("assets/audio/music.wav");
    };
    
    ui.onQuitGame = [&]() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
    
    // Start with main menu
    ui.showMainMenu();
    audio.stopMusic(); // Don't play music in menu
    
    std::cout << "\nVoxel Pac-Man 3D - Press START to play!" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - prev_time);
        prev_time = current_time;
        
        // Skip game logic when UI is showing (not playing)
        if (ui.getState() != GameState::PLAYING) {
            // Just render UI
            goto render_frame;
        }
        
        // Eating animation timer
        eatAnimTime += dt * 8.0f;
        
        if (!gameOver && !pacman.isDead) {
            if (g_input_dir != Direction::NONE) {
                pacman.handleInput(g_input_dir, maze);
                g_input_dir = Direction::NONE;
            }
            pacman.continueMovement(maze);
            pacman.update(dt);
            
            if (pacman.collectPellet(maze)) {
                ghostEatBonus = 200;
                for (auto& ghost : ghosts) ghost.setFrightened(8.0f);
            }
            
            if (pacman.score != lastScore) {
                audio.playSound("assets/audio/eat.mp3");
                lastScore = pacman.score;
                std::cout << "Score: " << pacman.score << std::endl;
            }
            
            glm::ivec2 ppos(pacman.grid_x, pacman.grid_y);
            for (auto& ghost : ghosts) {
                ghost.updateAI(maze, ppos);
                ghost.update(dt);
                
                if (!ghost.isEaten && checkCollision(pacman, ghost)) {
                    if (ghost.mode == GhostMode::FRIGHTENED) {
                        ghost.isEaten = true;
                        pacman.score += ghostEatBonus;
                        std::cout << "Ate ghost! +" << ghostEatBonus << std::endl;
                        ghostEatBonus *= 2;
                    } else {
                        pacman.die();
                        audio.playSound("assets/audio/death.wav");
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
                    audio.stopMusic();
                    if (!gameOverPrinted) {
                        std::cout << "\n=== GAME OVER === Score: " << pacman.score << std::endl;
                        gameOverPrinted = true;
                        ui.showGameOver(pacman.score);
                    }
                } else {
                    pacman.respawn(maze);
                    for (int i = 0; i < 4; i++)
                        ghosts[i].respawn(maze, ghost_spawns[i][0], ghost_spawns[i][1]);
                    std::cout << "Lives: " << pacman.lives << std::endl;
                }
            }
        }
        
        render_frame:
        // Fixed third person camera following Pac-Man (doesn't rotate)
        camera.setupThirdPerson(pacman.world_pos, 0.0f, 10.0f, 8.0f);
        
        glClearColor(SKY_R, SKY_G, SKY_B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        shader.setVec3("colorTint", glm::vec3(1.0f));
        shader.setBool("useTexture", false);
        mazeRenderer.render(shader, camera);
        mazeRenderer.renderPellets(shader, maze);
        
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = camera.getProjectionMatrix();
        
        // Render voxel grass around maze
        shader.setVec3("colorTint", glm::vec3(1.0f));
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);
        int grassIdx = 0;
        for (const auto& pos : grassPositions) {
            glm::mat4 grassMat = glm::translate(glm::mat4(1.0f), pos);
            grassMat = glm::scale(grassMat, glm::vec3(1.9f, 0.4f, 1.9f)); // Bigger to fill gaps
            shader.setMat4("model", grassMat);
            // Alternate grass and dirt for variety
            if ((grassIdx++ % 7) == 0) {
                dirtMesh.draw();
            } else {
                grassMesh.draw();
            }
        }
        
        // Render trees with green/brown tint
        if (useTreeModel) {
            shader.setVec3("colorTint", glm::vec3(0.6f, 0.9f, 0.4f)); // Green tint for trees
            for (size_t i = 0; i < treePositions.size(); i++) {
                glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), treePositions[i]);
                modelMat = glm::scale(modelMat, glm::vec3(2.5f));
                treeModel.render(shader, modelMat, view, proj);
            }
        }
        
        // Render clouds (simple white puffs)
        shader.setVec3("colorTint", glm::vec3(1.0f));
        for (const auto& pos : cloudPositions) {
            glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
            modelMat = glm::scale(modelMat, glm::vec3(4.0f, 2.0f, 3.0f));
            shader.setMat4("model", modelMat);
            cloudMesh.draw();
            // Side puff
            modelMat = glm::translate(glm::mat4(1.0f), pos + glm::vec3(3.0f, -0.3f, 0.0f));
            modelMat = glm::scale(modelMat, glm::vec3(2.5f, 1.5f, 2.0f));
            shader.setMat4("model", modelMat);
            cloudMesh.draw();
        }
        
        if (!gameOver) {
            // Render Pac-Man with eating animation
            if (!pacman.isDead) {
                shader.setVec3("colorTint", glm::vec3(1.0f, 1.0f, 0.2f));
                if (usePacmanModel) {
                    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pacman.world_pos);
                    
                    // Rotation facing forward
                    float angle = directionToAngle(pacman.current_dir);
                    modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(0, 1, 0));
                    
                    // Eating animation: scale pulsing
                    float eatScale = 0.5f;
                    if (pacman.is_moving) {
                        float pulse = 0.05f * std::sin(eatAnimTime);
                        eatScale = 0.5f + pulse;
                    }
                    modelMat = glm::scale(modelMat, glm::vec3(eatScale));
                    
                    pacmanModel.render(shader, modelMat, view, proj);
                } else {
                    pacman.render(shader);
                }
            }
            
            // Render ghosts
            for (auto& ghost : ghosts) {
                if (!ghost.isEaten) {
                    glm::vec3 tint = (ghost.mode == GhostMode::FRIGHTENED) 
                        ? glm::vec3(0.2f, 0.2f, 1.0f)
                        : getGhostTint(ghost.ghost_type);
                    shader.setVec3("colorTint", tint);
                    
                    if (useGhostModel) {
                        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), ghost.world_pos);
                        float angle = directionToAngle(ghost.current_dir) + 180.0f;
                        modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(0, 1, 0));
                        modelMat = glm::scale(modelMat, glm::vec3(0.3f));
                        ghostModel.render(shader, modelMat, view, proj);
                    } else {
                        ghost.render(shader);
                    }
                }
            }
            
            shader.setVec3("colorTint", glm::vec3(1.0f));
        }
        
        // Render UI overlay
        ui.render(shader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    audio.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
