// MenuManager.cpp - Complete menu system implementation

#include "MenuManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sstream>

// Constructor
MenuManager::MenuManager(SDL_Renderer* rend, int width, int height)
    : renderer(rend), 
      screenWidth(width), 
      screenHeight(height),
      buttonSpacing(70),
      buttonTexture(nullptr),
      decorTexture(nullptr),
      textTexture(nullptr),
      inputTexture(nullptr),
      backgroundTexture(nullptr) {
    
    state.currentScreen = MenuScreen::MAIN_MENU;
    state.selectedButtonIndex = 0;
    state.menuActive = true;
    state.animationCounter = 0;
}

// Destructor
MenuManager::~MenuManager() {
    if (buttonTexture) SDL_DestroyTexture(buttonTexture);
    if (decorTexture) SDL_DestroyTexture(decorTexture);
    if (textTexture) SDL_DestroyTexture(textTexture);
    if (inputTexture) SDL_DestroyTexture(inputTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
}

// Load all menu assets
void MenuManager::loadMenuAssets() {
    SDL_Surface* surface = nullptr;
    
    // Load button sprites
    surface = IMG_Load("assets/ui/menu_buttons.png");
    if (surface) {
        buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded menu_buttons.png" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to load menu_buttons.png" << std::endl;
    }
    
    // Load decorations
    surface = IMG_Load("assets/ui/menu_decorations.png");
    if (surface) {
        decorTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded menu_decorations.png" << std::endl;
    }
    
    // Load text sprites
    surface = IMG_Load("assets/ui/menu_text.png");
    if (surface) {
        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded menu_text.png" << std::endl;
    }
    
    // Load input indicators
    surface = IMG_Load("assets/ui/menu_input.png");
    if (surface) {
        inputTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded menu_input.png" << std::endl;
    }
    
    // Load background
    surface = IMG_Load("assets/ui/menu_background.png");
    if (surface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded menu_background.png" << std::endl;
    }
    
    std::cout << "✓ All menu assets loaded!" << std::endl;
}

// Show main menu
void MenuManager::showMainMenu() {
    state.currentScreen = MenuScreen::MAIN_MENU;
    state.selectedButtonIndex = 0;
    buttons.clear();
    MainMenu::createButtons(buttons, screenWidth, screenHeight);
    
    // Set button callbacks
    if (buttons.size() > 0) buttons[0].onClicked = [this]() { /* Start Game */ };
    if (buttons.size() > 1) buttons[1].onClicked = [this]() { showSettings(); };
    if (buttons.size() > 2) buttons[2].onClicked = [this]() { showLeaderboard(); };
    if (buttons.size() > 3) buttons[3].onClicked = [this]() { /* Quit */ };
}

// Show pause menu
void MenuManager::showPauseMenu() {
    state.currentScreen = MenuScreen::PAUSE_MENU;
    state.selectedButtonIndex = 0;
    buttons.clear();
    PauseMenu::createButtons(buttons, screenWidth, screenHeight);
    
    // Set button callbacks
    if (buttons.size() > 0) buttons[0].onClicked = [this]() { state.currentScreen = MenuScreen::GAME_RUNNING; };
    if (buttons.size() > 1) buttons[1].onClicked = [this]() { showSettings(); };
    if (buttons.size() > 2) buttons[2].onClicked = [this]() { showMainMenu(); };
}

// Show settings menu
void MenuManager::showSettings() {
    state.currentScreen = MenuScreen::SETTINGS;
    state.selectedButtonIndex = 0;
    buttons.clear();
    SettingsMenu::createButtons(buttons, screenWidth, screenHeight);
    
    // Back button callback
    if (buttons.size() > 0) {
        buttons[0].onClicked = [this]() { showMainMenu(); };
    }
}

// Show leaderboard
void MenuManager::showLeaderboard() {
    state.currentScreen = MenuScreen::LEADERBOARD;
}

// Show game over screen
void MenuManager::showGameOver(int finalScore) {
    state.currentScreen = MenuScreen::GAME_OVER;
    state.selectedButtonIndex = 0;
    buttons.clear();
    GameOverMenu::createButtons(buttons, screenWidth, screenHeight);
    
    // Set button callbacks
    if (buttons.size() > 0) buttons[0].onClicked = [this]() { state.currentScreen = MenuScreen::GAME_RUNNING; };
    if (buttons.size() > 1) buttons[1].onClicked = [this]() { showMainMenu(); };
    if (buttons.size() > 2) buttons[2].onClicked = [this]() { /* Quit */ };
}

// Handle mouse motion
void MenuManager::handleMouseMotion(int x, int y) {
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].hovered = buttons[i].contains(x, y);
        if (buttons[i].hovered) {
            state.selectedButtonIndex = i;
        }
    }
}

// Handle mouse click
void MenuManager::handleMouseClick(int x, int y) {
    for (auto& btn : buttons) {
        if (btn.contains(x, y)) {
            btn.pressed = true;
            if (btn.onClicked) {
                btn.onClicked();
            }
        }
    }
}

// Handle keyboard
void MenuManager::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (state.selectedButtonIndex > 0) {
                state.selectedButtonIndex--;
            }
            break;
            
        case SDLK_DOWN:
            if (state.selectedButtonIndex < (int)buttons.size() - 1) {
                state.selectedButtonIndex++;
            }
            break;
            
        case SDLK_RETURN:
        case SDLK_SPACE:
            if (state.selectedButtonIndex < (int)buttons.size()) {
                auto& btn = buttons[state.selectedButtonIndex];
                if (btn.onClicked) {
                    btn.onClicked();
                }
            }
            break;
            
        case SDLK_ESCAPE:
            if (state.currentScreen == MenuScreen::PAUSE_MENU) {
                state.currentScreen = MenuScreen::GAME_RUNNING;
            } else if (state.currentScreen != MenuScreen::MAIN_MENU) {
                showMainMenu();
            }
            break;
    }
}

// Update animations
void MenuManager::updateAnimation() {
    state.animationCounter++;
    if (state.animationCounter > 1000) {
        state.animationCounter = 0;
    }
}

// Main render function
void MenuManager::render() {
    renderBackground();
    
    switch (state.currentScreen) {
        case MenuScreen::MAIN_MENU:
            renderMainMenu();
            break;
        case MenuScreen::PAUSE_MENU:
            renderPauseMenu();
            break;
        case MenuScreen::SETTINGS:
            renderSettings();
            break;
        case MenuScreen::LEADERBOARD:
            renderLeaderboard();
            break;
        case MenuScreen::GAME_OVER:
            renderGameOver();
            break;
        default:
            break;
    }
}

// Render background
void MenuManager::renderBackground() {
    // Draw repeating background pattern
    int tileSize = 64;
    for (int y = 0; y < screenHeight; y += tileSize) {
        for (int x = 0; x < screenWidth; x += tileSize) {
            SDL_Rect dst = {x, y, tileSize, tileSize};
            // In real implementation, use backgroundTexture
            SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
            SDL_RenderFillRect(renderer, &dst);
        }
    }
    
    // Render decorative border
    renderDecorations();
}

// Render decorations
void MenuManager::renderDecorations() {
    if (!decorTexture) return;
    
    // Top border
    SDL_Rect src = {0, 0, 512, 20};
    SDL_Rect dst = {(screenWidth - 512) / 2, 20, 512, 20};
    SDL_RenderCopy(renderer, decorTexture, &src, &dst);
    
    // Bottom border
    src = {0, 140, 512, 20};
    dst = {(screenWidth - 512) / 2, screenHeight - 40, 512, 20};
    SDL_RenderCopy(renderer, decorTexture, &src, &dst);
    
    // Corner accents
    int frame = (state.animationCounter / 10) % 3;
    src = MenuDecorationSprites::cornerAccentFrames[frame];
    
    // Top-left corner
    dst = {10, 10, 40, 24};
    SDL_RenderCopy(renderer, decorTexture, &src, &dst);
    
    // Top-right corner
    dst = {screenWidth - 50, 10, 40, 24};
    SDL_RenderCopy(renderer, decorTexture, &src, &dst);
}

// Render main menu
void MenuManager::renderMainMenu() {
    // Draw title
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect titleRect = {screenWidth / 2 - 150, 50, 300, 60};
    SDL_RenderFillRect(renderer, &titleRect);
    
    // Draw buttons
    for (size_t i = 0; i < buttons.size(); i++) {
        renderButton(buttons[i], MenuButtonSprites::ButtonType(i));
    }
}

// Render button
void MenuManager::renderButton(const MenuButton& button, int buttonType) {
    if (!buttonTexture) return;
    
    // Determine button state
    MenuButtonSprites::ButtonState state = MenuButtonSprites::NORMAL;
    if (button.pressed) {
        state = MenuButtonSprites::ACTIVE;
    } else if (button.hovered) {
        state = MenuButtonSprites::HOVER;
    }
    
    // Get sprite coordinates
    Rect spriteCoords = MenuButtonSprites::buttons[buttonType][state];
    
    SDL_Rect src = {spriteCoords.x, spriteCoords.y, spriteCoords.w, spriteCoords.h};
    SDL_Rect dst = {button.x, button.y, button.width, button.height};
    
    SDL_RenderCopy(renderer, buttonTexture, &src, &dst);
    
    // Draw text on button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // In real implementation, render text with font
}

// Render pause menu
void MenuManager::renderPauseMenu() {
    // Semi-transparent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    
    // Draw pause panel background
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_Rect panel = {
        screenWidth / 2 - 150,
        screenHeight / 2 - 150,
        300, 300
    };
    SDL_RenderFillRect(renderer, &panel);
    
    // Draw buttons
    for (size_t i = 0; i < buttons.size(); i++) {
        renderButton(buttons[i], 0);  // Reuse button type
    }
}

// Render settings menu
void MenuManager::renderSettings() {
    // Title
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect titleRect = {screenWidth / 2 - 150, 50, 300, 60};
    SDL_RenderFillRect(renderer, &titleRect);
    
    // Settings list
    int startY = 150;
    int lineHeight = 50;
    
    // Difficulty setting
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect settingRect = {
        screenWidth / 2 - 150,
        startY,
        300, 40
    };
    SDL_RenderDrawRect(renderer, &settingRect);
    
    // Volume setting
    settingRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &settingRect);
    
    // Music setting
    settingRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &settingRect);
    
    // Back button
    if (!buttons.empty()) {
        renderButton(buttons[0], 0);
    }
}

// Render leaderboard
void MenuManager::renderLeaderboard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect titleRect = {screenWidth / 2 - 150, 50, 300, 60};
    SDL_RenderFillRect(renderer, &titleRect);
    
    // Draw leaderboard entries
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    
    // Sample leaderboard
    const char* leaders[] = {
        "1. Player One    10000",
        "2. Player Two     8500",
        "3. Player Three   7200",
        "4. Player Four    6800",
        "5. Player Five    5500"
    };
    
    int startY = 150;
    int lineHeight = 60;
    
    for (int i = 0; i < 5; i++) {
        SDL_Rect entryRect = {
            screenWidth / 2 - 150,
            startY + i * lineHeight,
            300, 50
        };
        SDL_RenderDrawRect(renderer, &entryRect);
    }
}

// Render game over screen
void MenuManager::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect titleRect = {screenWidth / 2 - 150, 50, 300, 60};
    SDL_RenderFillRect(renderer, &titleRect);
    
    // Score display
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect scoreRect = {screenWidth / 2 - 100, 150, 200, 40};
    SDL_RenderDrawRect(renderer, &scoreRect);
    
    // Buttons
    for (size_t i = 0; i < buttons.size(); i++) {
        renderButton(buttons[i], 0);
    }
}