#pragma once

#include <map>
#include <string>
#include <vector>

// SDL2 compatibility
struct Rect {
    int x, y, w, h;
};

/**
 * SPRITE SHEET COORDINATE SYSTEM
 * All coordinates are in pixels (x, y, width, height)
 * Origin (0,0) is top-left of each sprite sheet image
 */

// ============================================================================
// 1. PAC-MAN ANIMATION SPRITES (pacman_animation.png)
// ============================================================================
// Sprite Sheet: 384x128 pixels (12 sprites @ 32x32 each)
// Format: 3 frames per direction x 4 directions

namespace PacManSprites {
    constexpr int FRAME_WIDTH = 32;
    constexpr int FRAME_HEIGHT = 32;
    constexpr int ANIMATION_FRAMES = 3;
    
    enum Direction {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };
    
    // Up direction (frames 0, 1, 2)
    constexpr Rect UP_FRAME_0 = {0, 0, 32, 32};
    constexpr Rect UP_FRAME_1 = {32, 0, 32, 32};
    constexpr Rect UP_FRAME_2 = {64, 0, 32, 32};
    
    // Down direction (frames 0, 1, 2)
    constexpr Rect DOWN_FRAME_0 = {96, 0, 32, 32};
    constexpr Rect DOWN_FRAME_1 = {128, 0, 32, 32};
    constexpr Rect DOWN_FRAME_2 = {160, 0, 32, 32};
    
    // Left direction (frames 0, 1, 2)
    constexpr Rect LEFT_FRAME_0 = {192, 0, 32, 32};
    constexpr Rect LEFT_FRAME_1 = {224, 0, 32, 32};
    constexpr Rect LEFT_FRAME_2 = {256, 0, 32, 32};
    
    // Right direction (frames 0, 1, 2)
    constexpr Rect RIGHT_FRAME_0 = {288, 0, 32, 32};
    constexpr Rect RIGHT_FRAME_1 = {320, 0, 32, 32};
    constexpr Rect RIGHT_FRAME_2 = {352, 0, 32, 32};
    
    // Animation frame arrays
    static const Rect frames[4][3] = {
        {UP_FRAME_0, UP_FRAME_1, UP_FRAME_2},
        {DOWN_FRAME_0, DOWN_FRAME_1, DOWN_FRAME_2},
        {LEFT_FRAME_0, LEFT_FRAME_1, LEFT_FRAME_2},
        {RIGHT_FRAME_0, RIGHT_FRAME_1, RIGHT_FRAME_2}
    };
}

// ============================================================================
// 2. GHOST ANIMATION SPRITES (ghosts_animation.png)
// ============================================================================
// Sprite Sheet: 256x64 pixels (8 sprites @ 32x32 each)
// Format: 4 colors x 2 animation frames

namespace GhostSprites {
    constexpr int FRAME_WIDTH = 32;
    constexpr int FRAME_HEIGHT = 32;
    constexpr int ANIMATION_FRAMES = 2;
    
    enum GhostColor {
        RED = 0,
        BLUE = 1,
        ORANGE = 2,
        PINK = 3
    };
    
    // Red Ghost (frames 0, 1)
    constexpr Rect RED_FRAME_0 = {0, 0, 32, 32};
    constexpr Rect RED_FRAME_1 = {32, 0, 32, 32};
    
    // Blue Ghost (frames 0, 1)
    constexpr Rect BLUE_FRAME_0 = {64, 0, 32, 32};
    constexpr Rect BLUE_FRAME_1 = {96, 0, 32, 32};
    
    // Orange Ghost (frames 0, 1)
    constexpr Rect ORANGE_FRAME_0 = {128, 0, 32, 32};
    constexpr Rect ORANGE_FRAME_1 = {160, 0, 32, 32};
    
    // Pink Ghost (frames 0, 1)
    constexpr Rect PINK_FRAME_0 = {192, 0, 32, 32};
    constexpr Rect PINK_FRAME_1 = {224, 0, 32, 32};
    
    // Animation frame arrays
    static const Rect frames[4][2] = {
        {RED_FRAME_0, RED_FRAME_1},
        {BLUE_FRAME_0, BLUE_FRAME_1},
        {ORANGE_FRAME_0, ORANGE_FRAME_1},
        {PINK_FRAME_0, PINK_FRAME_1}
    };
}

// ============================================================================
// 3. MAZE WALL SPRITES (maze_walls.png)
// ============================================================================
// Sprite Sheet: 128x64 pixels (8 tiles @ 32x32 each)
// Different wall configurations for maze generation

namespace WallSprites {
    constexpr int TILE_WIDTH = 32;
    constexpr int TILE_HEIGHT = 32;
    
    enum WallType {
        EMPTY_CORRIDOR = 0,      // Empty space to walk through
        HORIZONTAL_WALL = 1,     // Straight horizontal wall
        VERTICAL_WALL = 2,       // Straight vertical wall
        CORNER_TL = 3,           // Top-left corner
        CORNER_TR = 4,           // Top-right corner
        CORNER_BL = 5,           // Bottom-left corner
        CORNER_BR = 6,           // Bottom-right corner
        T_JUNCTION = 7           // T-shaped junction
    };
    
    // Empty Corridor
    constexpr Rect EMPTY_CORRIDOR_SPRITE = {0, 0, 32, 32};
    
    // Horizontal Wall
    constexpr Rect HORIZONTAL_WALL_SPRITE = {32, 0, 32, 32};
    
    // Vertical Wall
    constexpr Rect VERTICAL_WALL_SPRITE = {64, 0, 32, 32};
    
    // Corners
    constexpr Rect CORNER_TL_SPRITE = {96, 0, 32, 32};
    constexpr Rect CORNER_TR_SPRITE = {0, 32, 32, 32};
    constexpr Rect CORNER_BL_SPRITE = {32, 32, 32, 32};
    constexpr Rect CORNER_BR_SPRITE = {64, 32, 32, 32};
    
    // T-Junction
    constexpr Rect T_JUNCTION_SPRITE = {96, 32, 32, 32};
    
    // Sprite lookup
    static const Rect tiles[8] = {
        EMPTY_CORRIDOR_SPRITE,
        HORIZONTAL_WALL_SPRITE,
        VERTICAL_WALL_SPRITE,
        CORNER_TL_SPRITE,
        CORNER_TR_SPRITE,
        CORNER_BL_SPRITE,
        CORNER_BR_SPRITE,
        T_JUNCTION_SPRITE
    };
}

// ============================================================================
// 4. COLLECTIBLE SPRITES (collectibles.png)
// ============================================================================
// Sprite Sheet: 160x32 pixels
// Pellets and power-ups with animation frames

namespace CollectibleSprites {
    enum CollectibleType {
        SMALL_PELLET = 0,
        MEDIUM_PELLET = 1,
        POWER_UP_FRAME_0 = 2,
        POWER_UP_FRAME_1 = 3,
        POWER_UP_FRAME_2 = 4
    };
    
    // Small Pellet (8x8)
    constexpr Rect SMALL_PELLET = {0, 12, 8, 8};
    
    // Medium Pellet (12x12)
    constexpr Rect MEDIUM_PELLET = {12, 10, 12, 12};
    
    // Power-up Animation (32x32 each)
    constexpr Rect POWER_UP_FRAME_0_SPRITE = {32, 0, 32, 32};
    constexpr Rect POWER_UP_FRAME_1_SPRITE = {64, 0, 32, 32};
    constexpr Rect POWER_UP_FRAME_2_SPRITE = {96, 0, 32, 32};
    
    // Power-up animation frames
    static const Rect powerUpFrames[3] = {
        POWER_UP_FRAME_0_SPRITE,
        POWER_UP_FRAME_1_SPRITE,
        POWER_UP_FRAME_2_SPRITE
    };
}

// ============================================================================
// 5. UI ELEMENT SPRITES (ui_elements.png)
// ============================================================================
// Sprite Sheet: 320x64 pixels
// Lives, score, text displays

namespace UISprites {
    // Life Icon (16x16)
    constexpr Rect LIFE_ICON = {0, 0, 16, 16};
    
    // Score Numbers (8x8 each)
    constexpr Rect NUMBER_0 = {20, 0, 8, 8};
    constexpr Rect NUMBER_1 = {28, 0, 8, 8};
    constexpr Rect NUMBER_2 = {36, 0, 8, 8};
    constexpr Rect NUMBER_3 = {44, 0, 8, 8};
    constexpr Rect NUMBER_4 = {52, 0, 8, 8};
    constexpr Rect NUMBER_5 = {60, 0, 8, 8};
    constexpr Rect NUMBER_6 = {68, 0, 8, 8};
    constexpr Rect NUMBER_7 = {76, 0, 8, 8};
    constexpr Rect NUMBER_8 = {84, 0, 8, 8};
    constexpr Rect NUMBER_9 = {92, 0, 8, 8};
    
    // Number array for easy access
    static const Rect numbers[10] = {
        NUMBER_0, NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4,
        NUMBER_5, NUMBER_6, NUMBER_7, NUMBER_8, NUMBER_9
    };
    
    // Text Labels (32x16 each)
    constexpr Rect TEXT_READY = {0, 16, 64, 16};
    constexpr Rect TEXT_GAME_OVER = {64, 16, 80, 16};
    constexpr Rect TEXT_LEVEL = {144, 16, 56, 16};
}

// ============================================================================
// 6. EFFECT SPRITES (effects_animations.png)
// ============================================================================
// Sprite Sheet: 128x128 pixels
// Special effects and animations

namespace EffectSprites {
    constexpr int FRAME_WIDTH = 32;
    constexpr int FRAME_HEIGHT = 32;
    
    // Ghost Vulnerable Effect (4 frames - blue/white flashing)
    constexpr Rect GHOST_VULNERABLE_0 = {0, 0, 32, 32};
    constexpr Rect GHOST_VULNERABLE_1 = {32, 0, 32, 32};
    constexpr Rect GHOST_VULNERABLE_2 = {64, 0, 32, 32};
    constexpr Rect GHOST_VULNERABLE_3 = {96, 0, 32, 32};
    
    static const Rect ghostVulnerableFrames[4] = {
        GHOST_VULNERABLE_0,
        GHOST_VULNERABLE_1,
        GHOST_VULNERABLE_2,
        GHOST_VULNERABLE_3
    };
    
    // Death Explosion (4 frames)
    constexpr Rect DEATH_EXPLOSION_0 = {0, 32, 32, 32};
    constexpr Rect DEATH_EXPLOSION_1 = {32, 32, 32, 32};
    constexpr Rect DEATH_EXPLOSION_2 = {64, 32, 32, 32};
    constexpr Rect DEATH_EXPLOSION_3 = {96, 32, 32, 32};
    
    static const Rect deathExplosionFrames[4] = {
        DEATH_EXPLOSION_0,
        DEATH_EXPLOSION_1,
        DEATH_EXPLOSION_2,
        DEATH_EXPLOSION_3
    };
    
    // Score Popups
    constexpr Rect SCORE_100 = {0, 64, 32, 32};
    constexpr Rect SCORE_200 = {32, 64, 32, 32};
    constexpr Rect SCORE_500 = {64, 64, 32, 32};
    constexpr Rect SCORE_1000 = {96, 64, 32, 32};
}

// ============================================================================
// HELPER FUNCTION FOR C++ SPRITE MANAGEMENT
// ============================================================================

class SpriteSheet {
public:
    SpriteSheet(const std::string& filepath) : filePath(filepath) {}
    
    // Get Pac-Man sprite by direction and frame
    Rect getPacManSprite(int direction, int frame) const {
        return PacManSprites::frames[direction][frame % PacManSprites::ANIMATION_FRAMES];
    }
    
    // Get Ghost sprite by color and frame
    Rect getGhostSprite(int color, int frame) const {
        return GhostSprites::frames[color][frame % GhostSprites::ANIMATION_FRAMES];
    }
    
    // Get Wall tile
    Rect getWallTile(int tileType) const {
        return WallSprites::tiles[tileType];
    }
    
    // Get Power-up animation frame
    Rect getPowerUpSprite(int frame) const {
        return CollectibleSprites::powerUpFrames[frame % 3];
    }
    
    // Get UI number
    Rect getNumberSprite(int digit) const {
        return UISprites::numbers[digit % 10];
    }
    
    const std::string& getFilePath() const { return filePath; }
    
private:
    std::string filePath;
};

// ============================================================================
// SPRITE SHEET FILE MAPPING FOR YOUR PROJECT
// ============================================================================
/*
 * PLACE THESE FILES IN YOUR PROJECT'S "assets/" FOLDER:
 * 
 * assets/sprites/pacman_animation.png          (384x128)
 * assets/sprites/ghosts_animation.png          (256x64)
 * assets/sprites/maze_walls.png                (128x64)
 * assets/sprites/collectibles.png              (160x32)
 * assets/sprites/ui_elements.png               (320x64)
 * assets/sprites/effects_animations.png        (128x128)
 * 
 * USAGE IN YOUR CODE:
 * 
 * SpriteSheet spriteSheet("assets/sprites/pacman_animation.png");
 * Rect pacmanRight = spriteSheet.getPacManSprite(PacManSprites::RIGHT, 0);
 * 
 * Rect ghostRed = spriteSheet.getGhostSprite(GhostSprites::RED, 0);
 * 
 * Rect wall = spriteSheet.getWallTile(WallSprites::CORNER_TL);
 */