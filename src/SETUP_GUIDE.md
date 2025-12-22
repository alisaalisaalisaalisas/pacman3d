# Pac-Man C++ Game - Complete Sprite Setup Guide

## 📁 Project Directory Structure

```
pacman-game/
├── CMakeLists.txt                    (Build configuration)
├── src/
│   ├── main.cpp                      (Entry point)
│   ├── SpriteManager.cpp             (Sprite loading & rendering)
│   ├── SpriteData.h                  (Sprite coordinates data)
│   ├── Game.h                        (Game class)
│   └── Game.cpp                      (Game implementation)
├── assets/
│   └── sprites/
│       ├── pacman_animation.png      (Pac-Man 12 frames)
│       ├── ghosts_animation.png      (4 Ghosts x 2 frames)
│       ├── maze_walls.png            (8 wall types)
│       ├── collectibles.png          (Pellets & power-ups)
│       ├── ui_elements.png           (UI & score display)
│       └── effects_animations.png    (Animations & effects)
└── build/                            (Compiled output)
```

## 📥 Step 1: Download All Sprite Sheets

Save these images to `assets/sprites/`:

1. **pacman_animation.png** (384x128)
   - Pac-Man in 4 directions × 3 animation frames each
   
2. **ghosts_animation.png** (256x64)
   - 4 ghost colors × 2 animation frames each
   
3. **maze_walls.png** (128x64)
   - 8 different wall tile configurations
   
4. **collectibles.png** (160x32)
   - Small pellets, medium pellets, power-up animation (3 frames)
   
5. **ui_elements.png** (320x64)
   - Numbers 0-9, life icon, text labels
   
6. **effects_animations.png** (128x128)
   - Ghost vulnerable effect, death explosion, score popups

## 🔧 Step 2: Install Dependencies

### On Linux (Ubuntu/Debian):
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

### On macOS:
```bash
brew install sdl2 sdl2_image
```

### On Windows:
Download SDL2 and SDL2_image development libraries from:
- https://www.libsdl.org/download-2.0.php
- https://www.libsdl.org/projects/SDL_image/

## 📝 Step 3: Copy Code Files

### SpriteData.h
- Header file with all sprite coordinate definitions
- Contains namespaces: PacManSprites, GhostSprites, WallSprites, etc.
- Place in `src/` directory

### SpriteManager.cpp
- Main sprite management and rendering system
- Loads all texture files
- Provides render functions for each sprite type
- Place in `src/` directory

## 🏗️ Step 4: CMakeLists.txt Setup

```cmake
cmake_minimum_required(VERSION 3.10)
project(PacMan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add executable
add_executable(pacman
    src/main.cpp
    src/SpriteManager.cpp
)

# Link libraries
target_link_libraries(pacman 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES}
)
```

## 🛠️ Step 5: Build Project

```bash
cd pacman-game
mkdir build
cd build
cmake ..
make
```

## ▶️ Step 6: Run Game

```bash
./pacman
```

Make sure `assets/` folder is in the same directory as the executable!

## 💻 Usage Reference

### Loading Sprites in Your Code

```cpp
// Create sprite manager
SpriteManager sprites(renderer);

// Render Pac-Man moving right, animation frame 0
sprites.renderPacMan(PacManSprites::RIGHT, 0, 100, 100);

// Render red ghost, animation frame 1
sprites.renderGhost(GhostSprites::RED, 1, 300, 300);

// Render wall tile
sprites.renderWallTile(WallSprites::CORNER_TL, 64, 64);

// Render pellet
sprites.renderPellet(200, 200);

// Render power-up with animation
sprites.renderPowerUp(frameCounter, 400, 400);
```

## 🎮 Animation Frame Updates

```cpp
// In your game loop
int animationCounter = 0;
int currentFrame = 0;

while (gameRunning) {
    animationCounter++;
    
    // Update frame every 10 game ticks
    if (animationCounter >= 10) {
        currentFrame++;
        animationCounter = 0;
    }
    
    // Render with current frame
    sprites.renderPacMan(direction, currentFrame, x, y);
}
```

## 🎯 Sprite Sheet Coordinates Reference

### Pac-Man Animation (4 directions, 3 frames each)
- **UP**: (0,0), (32,0), (64,0)
- **DOWN**: (96,0), (128,0), (160,0)
- **LEFT**: (192,0), (224,0), (256,0)
- **RIGHT**: (288,0), (320,0), (352,0)

### Ghosts (4 colors, 2 frames each)
- **RED**: (0,0), (32,0)
- **BLUE**: (64,0), (96,0)
- **ORANGE**: (128,0), (160,0)
- **PINK**: (192,0), (224,0)

### Walls (8 types)
- **EMPTY_CORRIDOR**: (0,0)
- **HORIZONTAL_WALL**: (32,0)
- **VERTICAL_WALL**: (64,0)
- **CORNER_TL**: (96,0)
- **CORNER_TR**: (0,32)
- **CORNER_BL**: (32,32)
- **CORNER_BR**: (64,32)
- **T_JUNCTION**: (96,32)

### Collectibles
- **SMALL_PELLET**: (0,12) - 8x8 pixels
- **MEDIUM_PELLET**: (12,10) - 12x12 pixels
- **POWER_UP**: (32-96, 0) - 3 animation frames

## ⚙️ Performance Tips

1. **Cache Animation Frames**: Pre-calculate which frame to render
2. **Sprite Batching**: Render multiple sprites in one pass
3. **Texture Memory**: All sprites loaded once at startup, reused
4. **60 FPS Cap**: `SDL_Delay(1000/60);` in game loop

## 🐛 Troubleshooting

### Sprites not showing?
- Check file paths are correct
- Verify sprite sheet files are in `assets/sprites/`
- Ensure texture coordinates match sprite positions

### Compilation errors?
- Install SDL2 development headers
- Check CMakeLists.txt paths
- Use correct C++ standard (C++17 or higher)

### Black screen?
- Verify renderer is created before loading sprites
- Check `SDL_RenderPresent()` is called
- Ensure coordinate system matches screen size

## 📚 Additional Resources

- SDL2 Documentation: https://wiki.libsdl.org/SDL2/APIByCategory
- Game loop tutorial: https://gameprogrammingpatterns.com/game-loop.html
- Sprite animation: https://www.gamedev.net/tutorials/programming/general/anatomy-of-a-sprite-sheet-r2959/

## ✅ Checklist

- [ ] All 6 sprite sheets downloaded
- [ ] `assets/sprites/` folder created
- [ ] SpriteData.h and SpriteManager.cpp in `src/`
- [ ] CMakeLists.txt configured
- [ ] SDL2 libraries installed
- [ ] Project compiles without errors
- [ ] Executable runs with `assets/` visible
- [ ] Sprites render on screen