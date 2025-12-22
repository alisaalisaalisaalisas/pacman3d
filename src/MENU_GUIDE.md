# Pac-Man Menu System - Complete UI Implementation Guide

## 📋 What's Included

### **3 Professional Menu UI Sprite Sheets**

1. **menu_design.png** - Reference design showing the complete menu layout
2. **menu_buttons.png** (800x200) - 4 button types × 4 states each
3. **menu_decorations.png** (512x384) - Borders, corners, decorations
4. **menu_text.png** - Text labels and titles (implementation)
5. **menu_input.png** - Selection indicators, sliders, checkboxes
6. **menu_background.png** - Background patterns and overlays

### **3 Complete Code Files**

1. **MenuUIData.h** - All sprite coordinate definitions
2. **MenuManager.h** - Complete menu system class
3. **MenuManager.cpp** - Full implementation with rendering

---

## 🎮 Menu Screens Included

### **1. Main Menu**
- Start Game button
- Settings button
- Leaderboard button
- Quit button
- Animated decorations
- Background pattern

### **2. Pause Menu** (In-Game)
- Resume button
- Settings button
- Exit to Menu button
- Semi-transparent overlay
- Current score display
- Lives remaining display

### **3. Settings Menu**
- Difficulty selector (Easy, Normal, Hard)
- Volume slider
- Music volume slider
- Number of ghosts selector
- Back button

### **4. Leaderboard Screen**
- Top 5 high scores display
- Player name column
- Score column
- Animated entry highlights
- Back/Exit button

### **5. Game Over Screen**
- "GAME OVER" title
- Final score display
- Try Again button
- Main Menu button
- Quit button

---

## 🎯 Button Types & States

### **Button States**
- **NORMAL** - Default button appearance
- **HOVER** - Mouse hovering over button (brighter)
- **ACTIVE** - Button clicked (pressed down effect)
- **DISABLED** - Grayed out/inactive

### **Button Types**
1. START GAME - Begin new game
2. SETTINGS - Access settings menu
3. LEADERBOARD - View high scores
4. QUIT - Exit game

---

## 📁 Project Structure

```
pacman-game/
├── src/
│   ├── MenuUIData.h              ✓ PROVIDED
│   ├── MenuManager.h             ✓ PROVIDED
│   ├── MenuManager.cpp           ✓ PROVIDED
│   ├── main.cpp                  (Your entry point)
│   └── Game.cpp                  (Game logic)
├── assets/
│   └── ui/
│       ├── menu_design.png       ✓ GENERATED (Reference)
│       ├── menu_buttons.png      ✓ GENERATED
│       ├── menu_decorations.png  ✓ GENERATED
│       ├── menu_text.png         (Optional, for text rendering)
│       ├── menu_input.png        (Optional, for UI elements)
│       └── menu_background.png   (Optional, for patterns)
└── CMakeLists.txt
```

---

## 🛠️ Quick Integration

### **Step 1: Copy Files**
```bash
# Copy menu code files
cp MenuUIData.h src/
cp MenuManager.h src/
cp MenuManager.cpp src/

# Create UI assets folder
mkdir assets/ui

# Copy menu sprites
cp menu_buttons.png assets/ui/
cp menu_decorations.png assets/ui/
```

### **Step 2: Update CMakeLists.txt**
```cmake
add_executable(pacman
    src/main.cpp
    src/Game.cpp
    src/SpriteManager.cpp
    src/MenuManager.cpp    # ADD THIS LINE
)
```

### **Step 3: Use in Main Game**
```cpp
#include "MenuManager.h"
#include "SpriteManager.h"

int main() {
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Pac-Man",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    
    // Create managers
    SpriteManager sprites(renderer);
    MenuManager menu(renderer, 800, 600);
    menu.loadMenuAssets();
    menu.showMainMenu();
    
    // Game state
    MenuScreen currentScreen = MenuScreen::MAIN_MENU;
    bool running = true;
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
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
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
```

---

## 🎨 Customization Guide

### **Change Button Layout**
```cpp
// In MenuManager::showMainMenu()
int centerX = screenWidth / 2 - 100;
int startY = screenHeight / 2 - 100;
int spacing = 70;  // Adjust spacing here

buttons.push_back({
    centerX, startY,
    200, 50,
    "START GAME",
    false, false,
    nullptr
});
```

### **Add Button Callbacks**
```cpp
// Set what happens when button is clicked
buttons[0].onClicked = [this]() {
    // Start game logic here
    std::cout << "Starting game..." << std::endl;
};

buttons[1].onClicked = [this]() {
    showSettings();  // Switch to settings menu
};

buttons[3].onClicked = [this]() {
    // Exit game
    running = false;
};
```

### **Customize Colors**
```cpp
// In MenuManager::renderMainMenu()
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Gray
SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);  // Dark background
```

### **Change Button Sizes**
```cpp
// In MenuButtonSprites namespace
constexpr int BUTTON_WIDTH = 200;   // Change to desired width
constexpr int BUTTON_HEIGHT = 50;   // Change to desired height
```

---

## 🔄 Menu Navigation Flow

```
MAIN MENU
├─→ START GAME → GAME (running)
├─→ SETTINGS → Settings Menu → Back to MAIN MENU
├─→ LEADERBOARD → Leaderboard Screen → Back to MAIN MENU
└─→ QUIT → Exit Application

GAME (running) → Press ESC or P
└─→ PAUSE MENU
    ├─→ RESUME → Back to GAME
    ├─→ SETTINGS → Settings Menu → Back to PAUSE MENU
    └─→ EXIT → Back to MAIN MENU

GAME (player loses)
└─→ GAME OVER SCREEN
    ├─→ TRY AGAIN → New Game
    ├─→ MAIN MENU → Back to MAIN MENU
    └─→ QUIT → Exit Application
```

---

## ⌨️ Keyboard Controls

| Key | Action |
|-----|--------|
| ↑ | Select previous menu item |
| ↓ | Select next menu item |
| ENTER / SPACE | Confirm selection |
| ESC | Back / Return to previous menu |
| Mouse Click | Click button directly |
| Mouse Move | Hover over buttons |

---

## 🎬 Animation Features

### **Animated Elements**
- Corner decorations pulse in/out (3 frames)
- Button highlight animation on hover
- Button press animation on click
- Score display animation on Game Over
- Leaderboard entry scroll animation

### **Animation Timing**
```cpp
// Update animations every frame
void MenuManager::updateAnimation() {
    state.animationCounter++;
    if (state.animationCounter > 1000) {
        state.animationCounter = 0;
    }
}

// Use in rendering
int frame = (state.animationCounter / 10) % 3;  // 3-frame animation
```

---

## 💾 Save/Load Configuration

### **Settings File Structure**
```json
{
  "difficulty": 1,    // 0=Easy, 1=Normal, 2=Hard
  "volume": 80,       // 0-100
  "music": 60,        // 0-100
  "ghosts": 4,        // 1-4
  "screenWidth": 800,
  "screenHeight": 600
}
```

### **Load Settings**
```cpp
void MenuManager::loadSettings() {
    // Read from JSON file
    // Apply to game configuration
    // Populate settings menu with values
}

void MenuManager::saveSettings() {
    // Save current settings to file
    // Use standard JSON library
}
```

---

## 🐛 Troubleshooting

### **Buttons not responding?**
- Check button coordinates are set correctly
- Verify mouse event handling is active
- Ensure renderer is passed correctly to MenuManager

### **Sprites not showing?**
- Verify `assets/ui/` folder exists
- Check sprite file paths match exactly
- Ensure SDL_image is properly initialized with `IMG_Init()`

### **Menu overlay flickering?**
- Check SDL_RenderClear() is called each frame
- Verify rendering order (background → buttons → decorations)
- Ensure SDL_RenderPresent() is called once per frame

### **Text not rendering?**
- Use SDL_ttf library for font rendering (optional)
- Alternative: pre-render text to sprite sheet
- Include text coordinates in MenuTextSprites namespace

---

## 📚 Advanced Features

### **High Score System**
```cpp
struct HighScore {
    std::string playerName;
    int score;
    int level;
    int dateTime;
};

std::vector<HighScore> leaderboard;

void loadLeaderboard() {
    // Read from file
    // Sort by score descending
    // Keep top 10
}

void saveScore(const std::string& name, int score) {
    leaderboard.push_back({name, score, currentLevel, time(0)});
    std::sort(leaderboard.begin(), leaderboard.end(),
        [](const HighScore& a, const HighScore& b) {
            return a.score > b.score;
        });
    if (leaderboard.size() > 10) {
        leaderboard.pop_back();
    }
}
```

### **Settings Persistence**
```cpp
void MenuManager::applySettings(const SettingsMenu::Setting& setting) {
    if (setting.name == "DIFFICULTY") {
        game->setDifficulty(setting.value);
    } else if (setting.name == "VOLUME") {
        audio->setVolume(setting.value);
    } else if (setting.name == "MUSIC") {
        audio->setMusicVolume(setting.value);
    }
}
```

---

## ✅ Checklist

- [ ] Menu code files copied to src/
- [ ] Menu sprite sheets in assets/ui/
- [ ] CMakeLists.txt updated with MenuManager.cpp
- [ ] MenuManager instantiated in main()
- [ ] Menu assets loaded before game loop
- [ ] Input events connected to menu
- [ ] All buttons have callbacks assigned
- [ ] Game state transitions working
- [ ] Pause menu integrated with game loop
- [ ] Game Over screen displays final score

---

## 📞 Support Files

All required files are ready to use:

✓ **MenuUIData.h** - Sprite coordinate definitions
✓ **MenuManager.h** - Menu system header
✓ **MenuManager.cpp** - Complete implementation
✓ **menu_buttons.png** - Button graphics
✓ **menu_decorations.png** - UI decorations
✓ **menu_design.png** - Reference design

Ready to integrate and customize for your Pac-Man game!
