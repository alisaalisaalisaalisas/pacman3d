// GameEndScreenManager.cpp - Complete implementation

#include "GameEndScreenManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sstream>
#include <iomanip>

GameEndScreenManager::GameEndScreenManager(SDL_Renderer* rend, int width, int height)
    : renderer(rend),
      screenWidth(width),
      screenHeight(height),
      gameOverTexture(nullptr),
      levelCompleteTexture(nullptr),
      uiElementsTexture(nullptr),
      animationCounter(0),
      displayDelay(0) {
}

GameEndScreenManager::~GameEndScreenManager() {
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (levelCompleteTexture) SDL_DestroyTexture(levelCompleteTexture);
    if (uiElementsTexture) SDL_DestroyTexture(uiElementsTexture);
}

void GameEndScreenManager::loadAssets() {
    SDL_Surface* surface = nullptr;
    
    // Load game over screen texture
    surface = IMG_Load("assets/ui/game_over_screen.png");
    if (surface) {
        gameOverTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded game_over_screen.png" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to load game_over_screen.png" << std::endl;
    }
    
    // Load level complete screen texture
    surface = IMG_Load("assets/ui/level_complete_screen.png");
    if (surface) {
        levelCompleteTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded level_complete_screen.png" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to load level_complete_screen.png" << std::endl;
    }
    
    // Load UI elements texture
    surface = IMG_Load("assets/ui/game_end_ui_elements.png");
    if (surface) {
        uiElementsTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        std::cout << "✓ Loaded game_end_ui_elements.png" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to load game_end_ui_elements.png" << std::endl;
    }
}

void GameEndScreenManager::showGameOver(const GameEndScreenData::GameStats& stats) {
    screenData.screenType = 0;  // Game Over
    screenData.stats = stats;
    screenData.animationFrame = 0;
    animationCounter = 0;
    displayDelay = 30;  // 0.5 second delay before showing
    
    // Setup buttons
    gameOverButtons.clear();
    
    int centerX = screenWidth / 2 - 100;
    int startY = screenHeight - 100;
    int spacing = 250;
    
    // Try Again button
    gameOverButtons.push_back({
        centerX, startY, 200, 50,
        "TRY AGAIN",
        false, false,
        nullptr
    });
    
    // Main Menu button
    gameOverButtons.push_back({
        centerX + spacing, startY, 200, 50,
        "MAIN MENU",
        false, false,
        nullptr
    });
    
    // Quit button
    gameOverButtons.push_back({
        centerX + spacing * 2, startY, 200, 50,
        "QUIT",
        false, false,
        nullptr
    });
    
    std::cout << "Game Over Screen Shown - Final Score: " << stats.finalScore << std::endl;
}

void GameEndScreenManager::showLevelComplete(const GameEndScreenData::ScoreBreakdown& breakdown,
                                            int nextLevel) {
    screenData.screenType = 1;  // Level Complete
    screenData.scoreBreak = breakdown;
    screenData.animationFrame = 0;
    animationCounter = 0;
    displayDelay = 30;
    
    // Setup buttons
    levelCompleteButtons.clear();
    
    int centerX = screenWidth / 2 - 150;
    int startY = screenHeight - 100;
    
    // Next Level button
    levelCompleteButtons.push_back({
        centerX, startY, 300, 60,
        "NEXT LEVEL",
        false, false,
        nullptr
    });
    
    // Main Menu button
    levelCompleteButtons.push_back({
        centerX + 350, startY, 300, 60,
        "MAIN MENU",
        false, false,
        nullptr
    });
    
    std::cout << "Level Complete! Total Score: " << breakdown.totalScore << std::endl;
}

void GameEndScreenManager::showGameWon(int totalScore, int totalLevels) {
    screenData.screenType = 2;  // Game Won
    screenData.scoreBreak.totalScore = totalScore;
    screenData.scoreBreak.levelNumber = totalLevels;
    screenData.animationFrame = 0;
    animationCounter = 0;
    displayDelay = 30;
    
    // Setup buttons for game won (similar to level complete)
    levelCompleteButtons.clear();
    
    int centerX = screenWidth / 2 - 150;
    int startY = screenHeight - 100;
    
    // Main Menu button
    levelCompleteButtons.push_back({
        centerX + 50, startY, 300, 60,
        "MAIN MENU",
        false, false,
        nullptr
    });
    
    // Quit button
    levelCompleteButtons.push_back({
        centerX + 400, startY, 300, 60,
        "QUIT",
        false, false,
        nullptr
    });
    
    std::cout << "Game Won! Final Score: " << totalScore << std::endl;
}

void GameEndScreenManager::handleMouseMotion(int x, int y) {
    if (screenData.screenType == 0) {
        for (auto& btn : gameOverButtons) {
            btn.hovered = btn.contains(x, y);
        }
    } else {
        for (auto& btn : levelCompleteButtons) {
            btn.hovered = btn.contains(x, y);
        }
    }
}

void GameEndScreenManager::handleMouseClick(int x, int y) {
    if (screenData.screenType == 0) {
        for (auto& btn : gameOverButtons) {
            if (btn.contains(x, y) && btn.onClicked) {
                btn.onClicked();
            }
        }
    } else {
        for (auto& btn : levelCompleteButtons) {
            if (btn.contains(x, y) && btn.onClicked) {
                btn.onClicked();
            }
        }
    }
}

void GameEndScreenManager::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_RETURN:
        case SDLK_SPACE:
            getSelectedButton();
            break;
        case SDLK_ESCAPE:
            // Go back to main menu
            break;
    }
}

void GameEndScreenManager::update() {
    animationCounter++;
    screenData.animationFrame = (animationCounter / 10) % 3;
    
    if (displayDelay > 0) {
        displayDelay--;
    }
}

void GameEndScreenManager::render() {
    if (displayDelay > 0) {
        return;  // Don't render yet
    }
    
    // Draw background
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &bgRect);
    
    if (screenData.screenType == 0) {
        renderGameOver();
    } else if (screenData.screenType == 1 || screenData.screenType == 2) {
        renderLevelComplete();
    }
}

void GameEndScreenManager::renderGameOver() {
    // Draw title background (red)
    SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
    SDL_Rect titleBg = {50, 30, screenWidth - 100, 100};
    SDL_RenderFillRect(renderer, &titleBg);
    
    // Draw "GAME OVER" text (would use SDL_ttf or pre-rendered text)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &titleBg);
    
    // Render final score section
    renderScoreDisplay(screenData.stats.finalScore,
                      screenWidth / 2 - 150, 150);
    
    // Render statistics panel
    renderStatistics();
    
    // Render buttons
    renderButtons(gameOverButtons);
    
    // Render animations
    renderAnimatedElements();
}

void GameEndScreenManager::renderLevelComplete() {
    // Draw title background (gold/yellow)
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
    SDL_Rect titleBg = {50, 30, screenWidth - 100, 100};
    SDL_RenderFillRect(renderer, &titleBg);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &titleBg);
    
    // Render score display
    renderScoreDisplay(screenData.scoreBreak.totalScore,
                      screenWidth / 2 - 150, 150);
    
    // Render bonus breakdown
    renderBonusBreakdown();
    
    // Render medal based on score
    int medal = screenData.getMedalType(screenData.scoreBreak.totalScore);
    if (uiElementsTexture && medal >= 0) {
        SDL_Rect src = GameEndUISprites::medals[medal];
        SDL_Rect dst = {screenWidth - 100, 150, 80, 80};
        SDL_RenderCopy(renderer, uiElementsTexture, (SDL_Rect*)&src, &dst);
    }
    
    // Render buttons
    renderButtons(levelCompleteButtons);
    
    // Render animations
    renderAnimatedElements();
}

void GameEndScreenManager::renderScoreDisplay(int score, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect scoreBox = {x, y, 300, 80};
    SDL_RenderDrawRect(renderer, &scoreBox);
    SDL_RenderFillRect(renderer, &scoreBox);
    
    // Format score with commas
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(6) << score;
    std::string scoreStr = oss.str();
    
    // In real implementation, render text here
    // For now, just draw the box
}

void GameEndScreenManager::renderStatistics() {
    int statsX = 50;
    int statsY = 280;
    int lineHeight = 40;
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    
    // Level reached
    SDL_Rect levelRect = {statsX, statsY, screenWidth - 100, lineHeight};
    SDL_RenderDrawRect(renderer, &levelRect);
    
    // Pellets eaten
    levelRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &levelRect);
    
    // Ghosts defeated
    levelRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &levelRect);
    
    // Time spent
    levelRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &levelRect);
}

void GameEndScreenManager::renderBonusBreakdown() {
    int startY = 280;
    int lineHeight = 40;
    int labelX = 50;
    int valueX = screenWidth - 200;
    
    SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
    
    // Time bonus line
    SDL_Rect bonusRect = {labelX, startY, screenWidth - 100, lineHeight};
    SDL_RenderDrawRect(renderer, &bonusRect);
    
    // Pellet bonus
    bonusRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &bonusRect);
    
    // Ghost bonus
    bonusRect.y += lineHeight;
    SDL_RenderDrawRect(renderer, &bonusRect);
    
    // Total bonus
    bonusRect.y += lineHeight;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bonusRect);
}

void GameEndScreenManager::renderAnimatedElements() {
    if (!uiElementsTexture) return;
    
    // Render animated sparkles
    Rect sparkleRect = screenData.getSparkleFrame();
    SDL_Rect src = {sparkleRect.x, sparkleRect.y, sparkleRect.w, sparkleRect.h};
    
    // Sparkle positions around the screen
    int sparklePositions[4][2] = {
        {100, 100},
        {screenWidth - 100, 100},
        {100, screenHeight - 100},
        {screenWidth - 100, screenHeight - 100}
    };
    
    for (int i = 0; i < 4; i++) {
        SDL_Rect dst = {sparklePositions[i][0], sparklePositions[i][1], 40, 40};
        SDL_RenderCopy(renderer, uiElementsTexture, &src, &dst);
    }
}

void GameEndScreenManager::renderButtons(const std::vector<MenuButton>& buttons) {
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    
    for (const auto& btn : buttons) {
        SDL_Rect btnRect = {btn.x, btn.y, btn.width, btn.height};
        
        if (btn.hovered) {
            SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
        } else if (btn.pressed) {
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        }
        
        SDL_RenderFillRect(renderer, &btnRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &btnRect);
    }
}

MenuButton* GameEndScreenManager::getSelectedButton() {
    // Return first button for now
    if (screenData.screenType == 0) {
        return gameOverButtons.empty() ? nullptr : &gameOverButtons[0];
    } else {
        return levelCompleteButtons.empty() ? nullptr : &levelCompleteButtons[0];
    }
}