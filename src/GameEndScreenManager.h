// GameEndScreenManager.h - Complete game end screen system

#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "GameEndScreenData.h"
#include "MenuManager.h"

/**
 * Game End Screen Manager
 * Handles Game Over and Level Complete screens
 */

class GameEndScreenManager {
private:
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    
    // Screen textures
    SDL_Texture* gameOverTexture;
    SDL_Texture* levelCompleteTexture;
    SDL_Texture* uiElementsTexture;
    
    // Screen data
    GameEndScreenData screenData;
    
    // Animation
    int animationCounter;
    int displayDelay;  // Delay before showing screen
    
    // Buttons for each screen
    std::vector<MenuButton> gameOverButtons;
    std::vector<MenuButton> levelCompleteButtons;
    
public:
    GameEndScreenManager(SDL_Renderer* rend, int width, int height);
    ~GameEndScreenManager();
    
    // Load assets
    void loadAssets();
    
    // Show game over screen
    void showGameOver(const GameEndScreenData::GameStats& stats);
    
    // Show level complete screen
    void showLevelComplete(const GameEndScreenData::ScoreBreakdown& breakdown,
                          int nextLevel);
    
    // Show game won screen
    void showGameWon(int totalScore, int totalLevels);
    
    // Input handling
    void handleMouseMotion(int x, int y);
    void handleMouseClick(int x, int y);
    void handleKeyPress(SDL_Keycode key);
    
    // Update and render
    void update();
    void render();
    void renderGameOver();
    void renderLevelComplete();
    
    // Helper functions
    void renderScoreDisplay(int score, int x, int y);
    void renderStatistics();
    void renderBonusBreakdown();
    void renderAnimatedElements();
    void renderButtons(const std::vector<MenuButton>& buttons);
    
    // Get selected button
    MenuButton* getSelectedButton();
};

// ============================================================================
// IMPLEMENTATION EXAMPLE
// ============================================================================

/*
// Usage in your main game:

GameEndScreenManager endScreen(renderer, screenWidth, screenHeight);
endScreen.loadAssets();

// When player dies:
GameEndScreenData::GameStats stats = {
    currentScore,
    highScore,
    currentLevel,
    pelletsEaten,
    ghostsDefeated,
    gameTime,
    livesUsed
};
endScreen.showGameOver(stats);

// When player completes level:
GameEndScreenData::ScoreBreakdown breakdown = {
    levelBaseScore,
    calculateTimeBonus(timeSpent),
    calculatePelletBonus(pelletsEaten),
    calculateGhostBonus(ghostsDefeated),
    totalLevelScore,
    isNewHighScore,
    currentLevel
};
endScreen.showLevelComplete(breakdown, nextLevel);

// In game loop:
while (inEndScreen) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        endScreen.handleMouseMotion(event.motion.x, event.motion.y);
        endScreen.handleMouseClick(event.button.x, event.button.y);
        endScreen.handleKeyPress(event.key.keysym.sym);
    }
    
    endScreen.update();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    endScreen.render();
    SDL_RenderPresent(renderer);
    SDL_Delay(1000/60);
}
*/