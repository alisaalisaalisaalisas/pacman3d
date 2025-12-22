#pragma once

#include <map>
#include <string>
#include <vector>

/**
 * MENU UI SPRITE COORDINATES
 * All coordinates are in pixels (x, y, width, height)
 * Used for main menu, settings, leaderboard screens
 */

// ============================================================================
// 1. MENU BUTTON SPRITES (menu_buttons.png)
// ============================================================================
// Sprite Sheet: 800x200 pixels
// Format: 4 button types × 4 states each

namespace MenuButtonSprites {
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    
    enum ButtonType {
        START_GAME = 0,
        SETTINGS = 1,
        LEADERBOARD = 2,
        QUIT = 3
    };
    
    enum ButtonState {
        NORMAL = 0,      // Normal appearance
        HOVER = 1,       // Mouse hovering over
        ACTIVE = 2,      // Clicked/pressed
        DISABLED = 3     // Grayed out
    };
    
    // START GAME BUTTON (4 states)
    constexpr Rect START_NORMAL = {0, 0, 200, 50};
    constexpr Rect START_HOVER = {200, 0, 200, 50};
    constexpr Rect START_ACTIVE = {400, 0, 200, 50};
    constexpr Rect START_DISABLED = {600, 0, 200, 50};
    
    // SETTINGS BUTTON (4 states)
    constexpr Rect SETTINGS_NORMAL = {0, 50, 200, 50};
    constexpr Rect SETTINGS_HOVER = {200, 50, 200, 50};
    constexpr Rect SETTINGS_ACTIVE = {400, 50, 200, 50};
    constexpr Rect SETTINGS_DISABLED = {600, 50, 200, 50};
    
    // LEADERBOARD BUTTON (4 states)
    constexpr Rect LEADERBOARD_NORMAL = {0, 100, 200, 50};
    constexpr Rect LEADERBOARD_HOVER = {200, 100, 200, 50};
    constexpr Rect LEADERBOARD_ACTIVE = {400, 100, 200, 50};
    constexpr Rect LEADERBOARD_DISABLED = {600, 100, 200, 50};
    
    // QUIT BUTTON (4 states)
    constexpr Rect QUIT_NORMAL = {0, 150, 200, 50};
    constexpr Rect QUIT_HOVER = {200, 150, 200, 50};
    constexpr Rect QUIT_ACTIVE = {400, 150, 200, 50};
    constexpr Rect QUIT_DISABLED = {600, 150, 200, 50};
    
    // Button sprite lookup table
    static const Rect buttons[4][4] = {
        {START_NORMAL, START_HOVER, START_ACTIVE, START_DISABLED},
        {SETTINGS_NORMAL, SETTINGS_HOVER, SETTINGS_ACTIVE, SETTINGS_DISABLED},
        {LEADERBOARD_NORMAL, LEADERBOARD_HOVER, LEADERBOARD_ACTIVE, LEADERBOARD_DISABLED},
        {QUIT_NORMAL, QUIT_HOVER, QUIT_ACTIVE, QUIT_DISABLED}
    };
}

// ============================================================================
// 2. MENU DECORATIONS & BACKGROUNDS (menu_decorations.png)
// ============================================================================
// Sprite Sheet: 512x384 pixels
// Borders, corners, background elements

namespace MenuDecorationSprites {
    // Title/Header Area
    constexpr Rect HEADER_BACKGROUND = {0, 0, 512, 80};
    constexpr Rect TITLE_PACMAN = {0, 80, 512, 40};  // "PAC-MAN" text area
    
    // Borders
    constexpr Rect BORDER_TOP = {0, 120, 512, 20};
    constexpr Rect BORDER_BOTTOM = {0, 140, 512, 20};
    constexpr Rect BORDER_LEFT = {0, 160, 20, 200};
    constexpr Rect BORDER_RIGHT = {492, 160, 20, 200};
    
    // Corner Decorations
    constexpr Rect CORNER_TOP_LEFT = {0, 360, 40, 24};
    constexpr Rect CORNER_TOP_RIGHT = {472, 360, 40, 24};
    constexpr Rect CORNER_BOTTOM_LEFT = {0, 380, 40, 4};  // Placeholder
    constexpr Rect CORNER_BOTTOM_RIGHT = {472, 380, 40, 4};  // Placeholder
    
    // Animated Corner Accents (3 frames)
    constexpr Rect CORNER_ACCENT_0 = {50, 360, 40, 24};
    constexpr Rect CORNER_ACCENT_1 = {100, 360, 40, 24};
    constexpr Rect CORNER_ACCENT_2 = {150, 360, 40, 24};
    
    static const Rect cornerAccentFrames[3] = {
        CORNER_ACCENT_0,
        CORNER_ACCENT_1,
        CORNER_ACCENT_2
    };
    
    // Decorative Elements
    constexpr Rect GHOST_DECORATION_RED = {200, 360, 40, 24};
    constexpr Rect GHOST_DECORATION_BLUE = {250, 360, 40, 24};
    constexpr Rect GHOST_DECORATION_ORANGE = {300, 360, 40, 24};
    constexpr Rect GHOST_DECORATION_PINK = {350, 360, 40, 24};
    
    // Power-up Decoration (Glowing sphere)
    constexpr Rect POWERUP_DECORATION = {400, 360, 40, 24};
    
    // Semi-transparent overlay panel
    constexpr Rect MENU_PANEL_OVERLAY = {0, 200, 512, 160};
}

// ============================================================================
// 3. MENU TEXT SPRITES (menu_text.png)
// ============================================================================
// Large text labels and menu titles

namespace MenuTextSprites {
    // Menu screen titles
    constexpr Rect TITLE_MAIN_MENU = {0, 0, 200, 40};      // "MAIN MENU"
    constexpr Rect TITLE_SETTINGS = {200, 0, 200, 40};     // "SETTINGS"
    constexpr Rect TITLE_LEADERBOARD = {400, 0, 200, 40};  // "LEADERBOARD"
    constexpr Rect TITLE_GAME_OVER = {0, 40, 200, 40};     // "GAME OVER"
    constexpr Rect TITLE_PAUSE_MENU = {200, 40, 200, 40};  // "PAUSED"
    constexpr Rect TITLE_LEVEL_COMPLETE = {400, 40, 200, 40};  // "LEVEL COMPLETE"
    
    // Action text
    constexpr Rect TEXT_PRESS_START = {0, 80, 240, 20};    // "PRESS START"
    constexpr Rect TEXT_HIGH_SCORE = {0, 100, 160, 20};    // "HIGH SCORE"
    constexpr Rect TEXT_YOUR_SCORE = {160, 100, 160, 20};  // "YOUR SCORE"
    constexpr Rect TEXT_LEVEL = {0, 120, 120, 20};         // "LEVEL"
    constexpr Rect TEXT_LIVES = {120, 120, 120, 20};       // "LIVES"
    
    // Navigation hints
    constexpr Rect TEXT_UP_DOWN = {0, 140, 200, 20};       // "↑ ↓ SELECT"
    constexpr Rect TEXT_ENTER = {200, 140, 200, 20};       // "ENTER CONFIRM"
    constexpr Rect TEXT_ESC = {400, 140, 200, 20};         // "ESC BACK"
}

// ============================================================================
// 4. MENU INPUT INDICATORS (menu_input.png)
// ============================================================================
// Selected/highlighted indicators, checkboxes, sliders

namespace MenuInputSprites {
    // Selection indicator (arrow/highlight)
    constexpr Rect SELECTION_ARROW_LEFT = {0, 0, 20, 20};
    constexpr Rect SELECTION_ARROW_RIGHT = {20, 0, 20, 20};
    constexpr Rect SELECTION_HIGHLIGHT = {40, 0, 200, 50};
    
    // Checkbox states
    constexpr Rect CHECKBOX_UNCHECKED = {0, 50, 20, 20};
    constexpr Rect CHECKBOX_CHECKED = {20, 50, 20, 20};
    constexpr Rect CHECKBOX_DISABLED = {40, 50, 20, 20};
    
    // Radio buttons
    constexpr Rect RADIO_UNSELECTED = {0, 70, 20, 20};
    constexpr Rect RADIO_SELECTED = {20, 70, 20, 20};
    
    // Slider elements
    constexpr Rect SLIDER_BACKGROUND = {0, 90, 200, 10};
    constexpr Rect SLIDER_FILL = {0, 100, 100, 10};  // Adjustable width
    constexpr Rect SLIDER_HANDLE = {200, 90, 20, 30};
    
    // Difficulty icons
    constexpr Rect DIFFICULTY_EASY = {0, 130, 30, 30};
    constexpr Rect DIFFICULTY_MEDIUM = {30, 130, 30, 30};
    constexpr Rect DIFFICULTY_HARD = {60, 130, 30, 30};
}

// ============================================================================
// 5. MENU BACKGROUND PATTERNS (menu_background.png)
// ============================================================================
// Tiling background patterns for menu

namespace MenuBackgroundSprites {
    // Repeating tile patterns
    constexpr Rect MAZE_PATTERN_TILE = {0, 0, 64, 64};      // Repeating maze pattern
    constexpr Rect BRICK_PATTERN_TILE = {64, 0, 64, 64};    // Brick wall pattern
    constexpr Rect GRID_PATTERN_TILE = {128, 0, 64, 64};    // Grid pattern
    constexpr Rect DOTS_PATTERN_TILE = {192, 0, 64, 64};    // Dots pattern
    
    // Gradient overlays (semi-transparent)
    constexpr Rect GRADIENT_DARK = {0, 64, 256, 32};        // Dark gradient
    constexpr Rect GRADIENT_LIGHT = {256, 64, 256, 32};     // Light gradient
    constexpr Rect VIGNETTE_EDGE = {0, 96, 512, 32};        // Edge vignette
}

// ============================================================================
// 6. PAUSE MENU SPECIFIC (pause_menu.png)
// ============================================================================
// In-game pause menu elements

namespace PauseMenuSprites {
    // Pause menu buttons
    constexpr Rect RESUME_NORMAL = {0, 0, 200, 50};
    constexpr Rect RESUME_HOVER = {200, 0, 200, 50};
    
    constexpr Rect SETTINGS_PAUSE_NORMAL = {0, 50, 200, 50};
    constexpr Rect SETTINGS_PAUSE_HOVER = {200, 50, 200, 50};
    
    constexpr Rect EXIT_NORMAL = {0, 100, 200, 50};
    constexpr Rect EXIT_HOVER = {200, 100, 200, 50};
    
    // Pause panel background
    constexpr Rect PAUSE_PANEL = {0, 150, 400, 200};
    
    // Game stats display area
    constexpr Rect STATS_PANEL = {0, 350, 400, 100};
}

// ============================================================================
// HELPER FUNCTION FOR MENU SPRITE MANAGEMENT
// ============================================================================

struct Rect {
    int x, y, w, h;
};

class MenuSpriteSheet {
public:
    MenuSpriteSheet(const std::string& buttonsFile, 
                    const std::string& decorFile,
                    const std::string& textFile) 
        : buttonsPath(buttonsFile), 
          decorPath(decorFile),
          textPath(textFile) {}
    
    // Get button sprite by type and state
    Rect getMenuButton(int buttonType, int state) const {
        return MenuButtonSprites::buttons[buttonType][state];
    }
    
    // Get decoration sprite
    Rect getCornerDecoration(int frameIndex) const {
        return MenuDecorationSprites::cornerAccentFrames[frameIndex % 3];
    }
    
    // Get text sprite
    Rect getTextSprite(int textType) const {
        // Implementation depends on text organization
        return {0, 0, 200, 40};
    }
    
    const std::string& getButtonsPath() const { return buttonsPath; }
    const std::string& getDecorPath() const { return decorPath; }
    const std::string& getTextPath() const { return textPath; }
    
private:
    std::string buttonsPath;
    std::string decorPath;
    std::string textPath;
};