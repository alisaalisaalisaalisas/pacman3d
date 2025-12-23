#include "ui.h"
#include "../glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

UIManager::UIManager() {}

void UIManager::init() {
    buttonMesh = std::make_unique<Mesh>(createCube(glm::vec3(1.0f)));
    panelMesh = std::make_unique<Mesh>(createCube(glm::vec3(1.0f)));
}

void UIManager::setScreenSize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
}

void UIManager::createButtons() {
    buttons.clear();
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    
    switch (currentState) {
        case GameState::MAIN_MENU: {
            buttons.push_back({
                glm::vec3(centerX, centerY - 50, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.2f, 0.6f, 0.2f),
                glm::vec3(0.3f, 0.8f, 0.3f),
                "START",
                false,
                [this]() { if (onStartGame) onStartGame(); }
            });
            buttons.push_back({
                glm::vec3(centerX, centerY + 20, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.6f, 0.2f, 0.2f),
                glm::vec3(0.8f, 0.3f, 0.3f),
                "QUIT",
                false,
                [this]() { if (onQuitGame) onQuitGame(); }
            });
            break;
        }
        case GameState::PAUSED: {
            buttons.push_back({
                glm::vec3(centerX, centerY - 50, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.2f, 0.6f, 0.2f),
                glm::vec3(0.3f, 0.8f, 0.3f),
                "RESUME",
                false,
                [this]() { if (onResumeGame) onResumeGame(); }
            });
            buttons.push_back({
                glm::vec3(centerX, centerY + 20, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.6f, 0.6f, 0.2f),
                glm::vec3(0.8f, 0.8f, 0.3f),
                "MENU",
                false,
                [this]() { showMainMenu(); }
            });
            break;
        }
        case GameState::GAME_OVER: {
            buttons.push_back({
                glm::vec3(centerX, centerY + 30, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.2f, 0.6f, 0.2f),
                glm::vec3(0.3f, 0.8f, 0.3f),
                "RESTART",
                false,
                [this]() { if (onRestartGame) onRestartGame(); }
            });
            buttons.push_back({
                glm::vec3(centerX, centerY + 100, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.6f, 0.6f, 0.2f),
                glm::vec3(0.8f, 0.8f, 0.3f),
                "MENU",
                false,
                [this]() { showMainMenu(); }
            });
            break;
        }
        case GameState::WIN: {
            buttons.push_back({
                glm::vec3(centerX, centerY + 30, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.2f, 0.6f, 0.2f),
                glm::vec3(0.3f, 0.8f, 0.3f),
                "PLAY AGAIN",
                false,
                [this]() { if (onRestartGame) onRestartGame(); }
            });
            buttons.push_back({
                glm::vec3(centerX, centerY + 100, 0),
                glm::vec3(200, 50, 10),
                glm::vec3(0.6f, 0.6f, 0.2f),
                glm::vec3(0.8f, 0.8f, 0.3f),
                "MENU",
                false,
                [this]() { showMainMenu(); }
            });
            break;
        }
        default:
            break;
    }
}

void UIManager::showMainMenu() {
    currentState = GameState::MAIN_MENU;
    createButtons();
}

void UIManager::showPauseMenu() {
    currentState = GameState::PAUSED;
    createButtons();
}

void UIManager::showGameOver(int score) {
    currentState = GameState::GAME_OVER;
    finalScore = score;
    createButtons();
}

void UIManager::showWin(int score) {
    currentState = GameState::WIN;
    finalScore = score;
    createButtons();
}

void UIManager::hide() {
    currentState = GameState::PLAYING;
    buttons.clear();
}

void UIManager::update(float mouseX, float mouseY) {
    for (auto& button : buttons) {
        button.hovered = isPointInButton(mouseX, mouseY, button);
    }
}

bool UIManager::handleClick(float mouseX, float mouseY) {
    for (auto& button : buttons) {
        if (isPointInButton(mouseX, mouseY, button)) {
            if (button.onClick) {
                button.onClick();
            }
            return true;
        }
    }
    return false;
}

bool UIManager::isPointInButton(float x, float y, const UIButton& button) {
    float halfW = button.size.x / 2.0f;
    float halfH = button.size.y / 2.0f;
    return x >= button.position.x - halfW && x <= button.position.x + halfW &&
           y >= button.position.y - halfH && y <= button.position.y + halfH;
}

void UIManager::renderPanel(Shader& shader, const glm::vec3& pos, const glm::vec3& size, const glm::vec3& color) {
    // Use orthographic projection for UI
    glm::mat4 ortho = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    
    shader.use();
    shader.setMat4("projection", ortho);
    shader.setMat4("view", view);
    shader.setVec3("colorTint", color);
    shader.setBool("useTexture", false);
    
    // Create a flat quad at position with size
    float left = pos.x - size.x / 2.0f;
    float right = pos.x + size.x / 2.0f;
    float top = pos.y - size.y / 2.0f;
    float bottom = pos.y + size.y / 2.0f;
    
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    
    // Draw a simple colored rectangle using immediate-style with the existing mesh
    // Position the cube but flatten it
    model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    shader.setMat4("model", model);
    
    panelMesh->draw();
}

void UIManager::renderButton(Shader& shader, const UIButton& button) {
    glm::vec3 color = button.hovered ? button.hoverColor : button.color;
    renderPanel(shader, button.position, button.size, color);
}

void UIManager::render(Shader& shader) {
    if (currentState == GameState::PLAYING) return;
    
    // Disable depth test for UI
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    
    // Draw background panel
    glm::vec3 bgColor(0.1f, 0.1f, 0.15f);
    renderPanel(shader, glm::vec3(centerX, centerY, -10), glm::vec3(400, 350, 5), bgColor);
    
    // Draw title area
    glm::vec3 titleColor;
    switch (currentState) {
        case GameState::MAIN_MENU:
            titleColor = glm::vec3(1.0f, 0.8f, 0.0f); // Yellow
            renderPanel(shader, glm::vec3(centerX, centerY - 130, 0), glm::vec3(350, 60, 8), titleColor);
            break;
        case GameState::PAUSED:
            titleColor = glm::vec3(0.3f, 0.5f, 0.9f); // Blue
            renderPanel(shader, glm::vec3(centerX, centerY - 130, 0), glm::vec3(300, 60, 8), titleColor);
            break;
        case GameState::GAME_OVER:
            titleColor = glm::vec3(0.8f, 0.2f, 0.2f); // Red
            renderPanel(shader, glm::vec3(centerX, centerY - 130, 0), glm::vec3(350, 60, 8), titleColor);
            // Score panel
            renderPanel(shader, glm::vec3(centerX, centerY - 50, 0), glm::vec3(250, 40, 8), glm::vec3(0.2f, 0.2f, 0.3f));
            break;
        case GameState::WIN:
            titleColor = glm::vec3(0.9f, 0.7f, 0.0f); // Gold
            renderPanel(shader, glm::vec3(centerX, centerY - 130, 0), glm::vec3(350, 60, 8), titleColor);
            // Score panel
            renderPanel(shader, glm::vec3(centerX, centerY - 50, 0), glm::vec3(250, 40, 8), glm::vec3(0.2f, 0.3f, 0.2f));
            break;
        default:
            break;
    }
    
    // Draw buttons
    for (const auto& button : buttons) {
        renderButton(shader, button);
    }
    
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
