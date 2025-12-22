// MenuManager.h - Complete menu system for Pac-Man game

#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <functional>
#include "MenuUIData.h"

/**
 * Complete Menu System for Pac-Man Game
 * Handles main menu, pause menu, settings, leaderboard
 */

// Menu Screen Types
enum class MenuScreen {
    MAIN_MENU = 0,
    GAME_RUNNING = 1,
    PAUSE_MENU = 2,
    SETTINGS = 3,
    LEADERBOARD = 4,
    GAME_OVER = 5
};

// Menu Button Structure
struct MenuButton {
    int x, y;
    int width, height;
    std::string label;
    bool hovered;
    bool pressed;
    std::function<void()> onClicked;
    
    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

// Menu State Structure
struct MenuState {
    MenuScreen currentScreen;
    int selectedButtonIndex;
    bool menuActive;
    int animationCounter;
};

class MenuManager {
private:
    SDL_Renderer* renderer;
    MenuState state;
    std::vector<MenuButton> buttons;
    
    // Sprite textures
    SDL_Texture* buttonTexture;
    SDL_Texture* decorTexture;
    SDL_Texture* textTexture;
    SDL_Texture* inputTexture;
    SDL_Texture* backgroundTexture;
    
    // Menu settings
    int screenWidth;
    int screenHeight;
    int buttonSpacing;

public:
    MenuManager(SDL_Renderer* rend, int width, int height);
    ~MenuManager();
    
    // Load menu assets
    void loadMenuAssets();
    
    // Menu screen management
    void showMainMenu();
    void showPauseMenu();
    void showSettings();
    void showLeaderboard();
    void showGameOver(int finalScore);
    
    // Input handling
    void handleMouseMotion(int x, int y);
    void handleMouseClick(int x, int y);
    void handleKeyPress(SDL_Keycode key);
    
    // Rendering
    void render();
    void renderMainMenu();
    void renderPauseMenu();
    void renderSettings();
    void renderLeaderboard();
    void renderGameOver();
    
    // Helper functions
    void renderButton(const MenuButton& button, int buttonType);
    void renderDecorations();
    void renderBackground();
    void updateAnimation();
    
    // State queries
    MenuScreen getCurrentScreen() const { return state.currentScreen; }
    bool isMenuActive() const { return state.menuActive; }
    int getSelectedButton() const { return state.selectedButtonIndex; }
};

// ============================================================================
// BUTTON DEFINITIONS FOR EACH MENU SCREEN
// ============================================================================

class MainMenu {
public:
    static void createButtons(std::vector<MenuButton>& buttons, int screenWidth, int screenHeight) {
        int centerX = screenWidth / 2 - 100;  // 200px button width
        int startY = screenHeight / 2 - 100;
        int spacing = 70;
        
        // Start Game Button
        buttons.push_back({
            centerX, startY,
            200, 50,
            "START GAME",
            false, false,
            nullptr
        });
        
        // Settings Button
        buttons.push_back({
            centerX, startY + spacing,
            200, 50,
            "SETTINGS",
            false, false,
            nullptr
        });
        
        // Leaderboard Button
        buttons.push_back({
            centerX, startY + spacing * 2,
            200, 50,
            "LEADERBOARD",
            false, false,
            nullptr
        });
        
        // Quit Button
        buttons.push_back({
            centerX, startY + spacing * 3,
            200, 50,
            "QUIT",
            false, false,
            nullptr
        });
    }
};

class PauseMenu {
public:
    static void createButtons(std::vector<MenuButton>& buttons, int screenWidth, int screenHeight) {
        int centerX = screenWidth / 2 - 100;
        int startY = screenHeight / 2 - 50;
        int spacing = 70;
        
        // Resume Button
        buttons.push_back({
            centerX, startY,
            200, 50,
            "RESUME",
            false, false,
            nullptr
        });
        
        // Settings Button
        buttons.push_back({
            centerX, startY + spacing,
            200, 50,
            "SETTINGS",
            false, false,
            nullptr
        });
        
        // Exit to Menu Button
        buttons.push_back({
            centerX, startY + spacing * 2,
            200, 50,
            "EXIT MENU",
            false, false,
            nullptr
        });
    }
};

class GameOverMenu {
public:
    static void createButtons(std::vector<MenuButton>& buttons, int screenWidth, int screenHeight) {
        int centerX = screenWidth / 2 - 100;
        int startY = screenHeight / 2 + 50;
        int spacing = 70;
        
        // Try Again Button
        buttons.push_back({
            centerX, startY,
            200, 50,
            "TRY AGAIN",
            false, false,
            nullptr
        });
        
        // Main Menu Button
        buttons.push_back({
            centerX, startY + spacing,
            200, 50,
            "MAIN MENU",
            false, false,
            nullptr
        });
        
        // Quit Button
        buttons.push_back({
            centerX, startY + spacing * 2,
            200, 50,
            "QUIT",
            false, false,
            nullptr
        });
    }
};

class SettingsMenu {
public:
    struct Setting {
        std::string name;
        int value;
        int minValue;
        int maxValue;
    };
    
    static void createSettings(std::vector<Setting>& settings) {
        settings.push_back({"DIFFICULTY", 1, 0, 2});  // 0=Easy, 1=Normal, 2=Hard
        settings.push_back({"VOLUME", 80, 0, 100});
        settings.push_back({"MUSIC", 60, 0, 100});
        settings.push_back({"GHOSTS", 4, 1, 4});
    }
    
    static void createButtons(std::vector<MenuButton>& buttons, int screenWidth, int screenHeight) {
        int centerX = screenWidth / 2 - 100;
        int startY = screenHeight / 2 + 100;
        
        // Back Button
        buttons.push_back({
            centerX, startY,
            200, 50,
            "BACK",
            false, false,
            nullptr
        });
    }
};

// ============================================================================
// USAGE EXAMPLE
// ============================================================================

/*
// In your main game loop:

MenuManager menu(renderer, screenWidth, screenHeight);
menu.loadMenuAssets();
menu.showMainMenu();

SDL_Event event;
while (gameRunning) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                menu.handleMouseMotion(event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                menu.handleMouseClick(event.button.x, event.button.y);
                break;
            case SDL_KEYDOWN:
                menu.handleKeyPress(event.key.keysym.sym);
                break;
        }
    }
    
    // Update
    menu.updateAnimation();
    
    // Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    menu.render();
    SDL_RenderPresent(renderer);
    
    SDL_Delay(1000 / 60);  // 60 FPS
}
*/