#pragma once

#include <map>
#include <string>
#include <vector>

/**
 * GAME END SCREEN SPRITE COORDINATES
 * Used for Game Over and Level Complete screens
 */

struct Rect {
    int x, y, w, h;
};

// ============================================================================
// 1. GAME OVER SCREEN SPRITES (game_over_screen.png)
// ============================================================================

namespace GameOverSprites {
    // Game Over Title
    constexpr Rect TITLE_GAME_OVER = {0, 0, 400, 80};
    
    // Score Display Area
    constexpr Rect SCORE_DISPLAY = {100, 100, 600, 100};
    constexpr Rect SCORE_LABEL = {0, 200, 200, 30};
    
    // Statistics Panel
    constexpr Rect STATS_PANEL = {50, 240, 700, 150};
    constexpr Rect STATS_LEVEL = {60, 250, 300, 30};
    constexpr Rect STATS_LIVES_USED = {60, 290, 300, 30};
    constexpr Rect STATS_ENEMIES = {60, 330, 300, 30};
    constexpr Rect STATS_TIME = {400, 250, 300, 30};
    constexpr Rect STATS_PELLETS = {400, 290, 300, 30};
    
    // High Score Comparison
    constexpr Rect HIGH_SCORE_DISPLAY = {100, 410, 600, 80};
    
    // Game Over Buttons
    constexpr Rect BUTTON_TRY_AGAIN = {50, 520, 200, 50};
    constexpr Rect BUTTON_MAIN_MENU = {300, 520, 200, 50};
    constexpr Rect BUTTON_QUIT = {550, 520, 200, 50};
    
    // Decorative Elements
    constexpr Rect SAD_PACMAN = {0, 600, 80, 80};
    constexpr Rect DECORATION_BLOCK_1 = {80, 600, 60, 60};
    constexpr Rect DECORATION_BLOCK_2 = {140, 600, 60, 60};
}

// ============================================================================
// 2. LEVEL COMPLETE / WIN SCREEN SPRITES (level_complete_screen.png)
// ============================================================================

namespace LevelCompleteSprites {
    // Title Sprites
    constexpr Rect TITLE_LEVEL_COMPLETE = {0, 0, 400, 80};
    constexpr Rect TITLE_YOU_WIN = {400, 0, 400, 80};
    
    // Victory Pac-Man Character
    constexpr Rect PACMAN_VICTORY = {0, 100, 120, 120};
    
    // Score Display
    constexpr Rect FINAL_SCORE_BOX = {150, 100, 500, 80};
    
    // Bonus Breakdown Panels
    constexpr Rect BONUS_TIME_LABEL = {50, 200, 300, 30};
    constexpr Rect BONUS_TIME_VALUE = {350, 200, 150, 30};
    
    constexpr Rect BONUS_PELLETS_LABEL = {50, 240, 300, 30};
    constexpr Rect BONUS_PELLETS_VALUE = {350, 240, 150, 30};
    
    constexpr Rect BONUS_GHOSTS_LABEL = {50, 280, 300, 30};
    constexpr Rect BONUS_GHOSTS_VALUE = {350, 280, 150, 30};
    
    constexpr Rect BONUS_TOTAL_LABEL = {50, 330, 300, 40};
    constexpr Rect BONUS_TOTAL_VALUE = {350, 330, 150, 40};
    
    // Next Level Info
    constexpr Rect NEXT_LEVEL_BOX = {50, 390, 700, 60};
    constexpr Rect NEXT_LEVEL_LABEL = {60, 400, 200, 40};
    constexpr Rect NEXT_LEVEL_NUMBER = {270, 400, 150, 40};
    
    // Buttons
    constexpr Rect BUTTON_NEXT_LEVEL = {50, 480, 300, 60};
    constexpr Rect BUTTON_MENU = {450, 480, 300, 60};
    
    // Medal/Achievement Icons
    constexpr Rect MEDAL_BRONZE = {0, 560, 60, 60};
    constexpr Rect MEDAL_SILVER = {70, 560, 60, 60};
    constexpr Rect MEDAL_GOLD = {140, 560, 60, 60};
    
    // Decorative sparkles (3 animation frames)
    constexpr Rect SPARKLE_0 = {210, 560, 40, 40};
    constexpr Rect SPARKLE_1 = {260, 560, 40, 40};
    constexpr Rect SPARKLE_2 = {310, 560, 40, 40};
    
    static const Rect sparkleFrames[3] = {
        SPARKLE_0,
        SPARKLE_1,
        SPARKLE_2
    };
}

// ============================================================================
// 3. GAME END UI ELEMENTS (game_end_ui_elements.png)
// ============================================================================

namespace GameEndUISprites {
    enum TextType {
        GAME_OVER_TEXT = 0,
        LEVEL_COMPLETE_TEXT = 1,
        YOU_WIN_TEXT = 2
    };
    
    enum BonusType {
        TIME_BONUS = 0,
        PELLET_BONUS = 1,
        GHOST_BONUS = 2
    };
    
    enum MedalType {
        MEDAL_BRONZE = 0,
        MEDAL_SILVER = 1,
        MEDAL_GOLD = 2
    };
    
    // Title Texts
    constexpr Rect TEXT_GAME_OVER = {0, 0, 300, 60};
    constexpr Rect TEXT_LEVEL_COMPLETE = {300, 0, 300, 60};
    constexpr Rect TEXT_YOU_WIN = {600, 0, 300, 60};
    
    // Bonus Labels
    constexpr Rect TEXT_TIME_BONUS = {0, 70, 200, 30};
    constexpr Rect TEXT_PELLET_BONUS = {200, 70, 200, 30};
    constexpr Rect TEXT_GHOST_BONUS = {400, 70, 200, 30};
    
    // Score Labels
    constexpr Rect TEXT_FINAL_SCORE = {0, 110, 200, 30};
    constexpr Rect TEXT_HIGH_SCORE = {200, 110, 200, 30};
    constexpr Rect TEXT_LEVEL = {400, 110, 200, 30};
    
    // Number Display Boxes (for dynamic numbers)
    constexpr Rect NUMBER_BOX_SMALL = {0, 150, 80, 40};
    constexpr Rect NUMBER_BOX_MEDIUM = {100, 150, 120, 40};
    constexpr Rect NUMBER_BOX_LARGE = {230, 150, 150, 40};
    
    // Medal Icons
    constexpr Rect MEDAL_BRONZE = {0, 200, 60, 60};
    constexpr Rect MEDAL_SILVER = {70, 200, 60, 60};
    constexpr Rect MEDAL_GOLD = {140, 200, 60, 60};
    
    static const Rect medals[3] = {
        MEDAL_BRONZE,
        MEDAL_SILVER,
        MEDAL_GOLD
    };
    
    // Sparkle/Star Effects (3 frames each)
    constexpr Rect SPARKLE_FRAME_0 = {210, 200, 40, 40};
    constexpr Rect SPARKLE_FRAME_1 = {260, 200, 40, 40};
    constexpr Rect SPARKLE_FRAME_2 = {310, 200, 40, 40};
    
    static const Rect sparkles[3] = {
        SPARKLE_FRAME_0,
        SPARKLE_FRAME_1,
        SPARKLE_FRAME_2
    };
    
    // Animated stars (for celebration)
    constexpr Rect STAR_FRAME_0 = {0, 260, 30, 30};
    constexpr Rect STAR_FRAME_1 = {40, 260, 30, 30};
    constexpr Rect STAR_FRAME_2 = {80, 260, 30, 30};
    
    static const Rect stars[3] = {
        STAR_FRAME_0,
        STAR_FRAME_1,
        STAR_FRAME_2
    };
}

// ============================================================================
// HELPER CLASS FOR GAME END SCREEN MANAGEMENT
// ============================================================================

class GameEndScreenData {
public:
    struct ScoreBreakdown {
        int levelScore;
        int timeBonus;
        int pelletBonus;
        int ghostBonus;
        int totalScore;
        bool isNewHighScore;
        int levelNumber;
    };
    
    struct GameStats {
        int finalScore;
        int highScore;
        int level;
        int pelletsEaten;
        int ghostsDefeated;
        int timeSpent;
        int livesLost;
    };
    
    GameEndScreenData() : screenType(0), animationFrame(0) {}
    
    // 0 = Game Over, 1 = Level Complete, 2 = Game Won
    int screenType;
    ScoreBreakdown scoreBreak;
    GameStats stats;
    int animationFrame;
    
    // Get medal based on score
    int getMedalType(int score) const {
        if (score >= 10000) return GameEndUISprites::MEDAL_GOLD;
        if (score >= 5000) return GameEndUISprites::MEDAL_SILVER;
        return GameEndUISprites::MEDAL_BRONZE;
    }
    
    // Get sparkle animation frame
    Rect getSparkleFrame() const {
        return GameEndUISprites::sparkles[animationFrame % 3];
    }
    
    // Get star animation frame
    Rect getStarFrame() const {
        return GameEndUISprites::stars[animationFrame % 3];
    }
};

// ============================================================================
// BUTTON DEFINITIONS FOR GAME END SCREENS
// ============================================================================

class GameOverButtons {
    enum ButtonType {
        TRY_AGAIN = 0,
        MAIN_MENU = 1,
        QUIT = 2
    };
};

class LevelCompleteButtons {
    enum ButtonType {
        NEXT_LEVEL = 0,
        MAIN_MENU = 1
    };
};