#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    WIN
};

struct UIButton {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;
    glm::vec3 hoverColor;
    std::string label;
    bool hovered = false;
    std::function<void()> onClick;
};

class UIManager {
public:
    UIManager();
    
    void init();
    void setScreenSize(int width, int height);
    
    void showMainMenu();
    void showPauseMenu();
    void showGameOver(int score);
    void showWin(int score);
    void hide();
    
    void update(float mouseX, float mouseY);
    bool handleClick(float mouseX, float mouseY);
    void render(Shader& shader);
    
    GameState getState() const { return currentState; }
    void setState(GameState state) { currentState = state; }
    
    // Callbacks
    std::function<void()> onStartGame;
    std::function<void()> onResumeGame;
    std::function<void()> onRestartGame;
    std::function<void()> onQuitGame;
    
private:
    GameState currentState = GameState::MAIN_MENU;
    std::vector<UIButton> buttons;
    std::unique_ptr<Mesh> buttonMesh;
    std::unique_ptr<Mesh> panelMesh;
    
    int screenWidth = 1280;
    int screenHeight = 720;
    int finalScore = 0;
    
    void createButtons();
    void renderButton(Shader& shader, const UIButton& button);
    void renderPanel(Shader& shader, const glm::vec3& pos, const glm::vec3& size, const glm::vec3& color);
    bool isPointInButton(float x, float y, const UIButton& button);
};

#endif // UI_H
