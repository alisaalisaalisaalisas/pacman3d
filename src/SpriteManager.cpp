// SpriteManager.cpp - Complete sprite loading and rendering implementation for SDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include "SpriteData.h"

/**
 * Complete Sprite Manager for Pac-Man Game
 * Handles loading, caching, and rendering all sprite sheets
 */

class SpriteManager {
private:
    std::map<std::string, SDL_Texture*> textures;
    SDL_Renderer* renderer;

public:
    SpriteManager(SDL_Renderer* rend) : renderer(rend) {
        loadAllSprites();
    }

    // Load all sprite sheets into memory
    void loadAllSprites() {
        // Load Pac-Man animation sprites
        loadTexture("pacman", "assets/sprites/pacman_animation.png");
        
        // Load Ghost sprites
        loadTexture("ghosts", "assets/sprites/ghosts_animation.png");
        
        // Load Maze walls
        loadTexture("walls", "assets/sprites/maze_walls.png");
        
        // Load Collectibles
        loadTexture("collectibles", "assets/sprites/collectibles.png");
        
        // Load UI elements
        loadTexture("ui", "assets/sprites/ui_elements.png");
        
        // Load Effects
        loadTexture("effects", "assets/sprites/effects_animations.png");
        
        std::cout << "✓ All sprite sheets loaded successfully!" << std::endl;
    }

    // Load individual texture
    bool loadTexture(const std::string& key, const std::string& filepath) {
        SDL_Surface* surface = IMG_Load(filepath.c_str());
        if (!surface) {
            std::cerr << "ERROR: Failed to load " << filepath << " - " 
                      << IMG_GetError() << std::endl;
            return false;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "ERROR: Failed to create texture from " << filepath << std::endl;
            return false;
        }

        textures[key] = texture;
        std::cout << "✓ Loaded: " << filepath << std::endl;
        return true;
    }

    // Render sprite with source rectangle
    void renderSprite(const std::string& sheetKey, const Rect& srcRect, 
                      int screenX, int screenY, int width = 32, int height = 32) {
        if (textures.find(sheetKey) == textures.end()) {
            std::cerr << "ERROR: Sprite sheet '" << sheetKey << "' not found!" << std::endl;
            return;
        }

        SDL_Rect src = {srcRect.x, srcRect.y, srcRect.w, srcRect.h};
        SDL_Rect dst = {screenX, screenY, width, height};
        
        SDL_RenderCopy(renderer, textures[sheetKey], &src, &dst);
    }

    // Render Pac-Man
    void renderPacMan(int direction, int animFrame, int screenX, int screenY) {
        Rect sprite = PacManSprites::frames[direction][animFrame % 3];
        renderSprite("pacman", sprite, screenX, screenY, 32, 32);
    }

    // Render Ghost
    void renderGhost(int color, int animFrame, int screenX, int screenY) {
        Rect sprite = GhostSprites::frames[color][animFrame % 2];
        renderSprite("ghosts", sprite, screenX, screenY, 32, 32);
    }

    // Render Wall Tile
    void renderWallTile(int wallType, int screenX, int screenY) {
        Rect sprite = WallSprites::tiles[wallType];
        renderSprite("walls", sprite, screenX, screenY, 32, 32);
    }

    // Render Pellet
    void renderPellet(int screenX, int screenY) {
        renderSprite("collectibles", CollectibleSprites::SMALL_PELLET, 
                     screenX, screenY, 8, 8);
    }

    // Render Power-Up
    void renderPowerUp(int animFrame, int screenX, int screenY) {
        Rect sprite = CollectibleSprites::powerUpFrames[animFrame % 3];
        renderSprite("collectibles", sprite, screenX, screenY, 32, 32);
    }

    // Render UI Life Icon
    void renderLifeIcon(int screenX, int screenY) {
        renderSprite("ui", UISprites::LIFE_ICON, screenX, screenY, 16, 16);
    }

    // Render Score Number
    void renderScoreNumber(int digit, int screenX, int screenY) {
        if (digit < 0 || digit > 9) return;
        renderSprite("ui", UISprites::numbers[digit], screenX, screenY, 8, 8);
    }

    // Render Effect Animation
    void renderGhostVulnerable(int animFrame, int screenX, int screenY) {
        Rect sprite = EffectSprites::ghostVulnerableFrames[animFrame % 4];
        renderSprite("effects", sprite, screenX, screenY, 32, 32);
    }

    void renderDeathExplosion(int animFrame, int screenX, int screenY) {
        Rect sprite = EffectSprites::deathExplosionFrames[animFrame % 4];
        renderSprite("effects", sprite, screenX, screenY, 32, 32);
    }

    // Cleanup
    ~SpriteManager() {
        for (auto& pair : textures) {
            if (pair.second) {
                SDL_DestroyTexture(pair.second);
            }
        }
        textures.clear();
    }
};

// ============================================================================
// EXAMPLE USAGE IN MAIN GAME LOOP
// ============================================================================

/*
int main(int argc, char* argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Pac-Man Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create sprite manager
    SpriteManager spriteManager(renderer);

    // Game variables
    int pacmanX = 100, pacmanY = 100;
    int pacmanDirection = PacManSprites::RIGHT;
    int pacmanAnimFrame = 0;
    int animationCounter = 0;

    int ghostX[4] = {300, 350, 400, 450};
    int ghostY[4] = {250, 250, 250, 250};
    int ghostAnimFrame[4] = {0, 0, 0, 0};

    // Game loop
    bool running = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        pacmanDirection = PacManSprites::UP;
                        pacmanY -= 32;
                        break;
                    case SDLK_DOWN:
                        pacmanDirection = PacManSprites::DOWN;
                        pacmanY += 32;
                        break;
                    case SDLK_LEFT:
                        pacmanDirection = PacManSprites::LEFT;
                        pacmanX -= 32;
                        break;
                    case SDLK_RIGHT:
                        pacmanDirection = PacManSprites::RIGHT;
                        pacmanX += 32;
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            }
        }

        // Update animation frames
        animationCounter++;
        if (animationCounter >= 10) {  // Change frame every 10 frames
            pacmanAnimFrame++;
            for (int i = 0; i < 4; i++) {
                ghostAnimFrame[i]++;
            }
            animationCounter = 0;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render game board (example)
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                if ((x + y) % 2 == 0) {
                    spriteManager.renderWallTile(WallSprites::HORIZONTAL_WALL, 
                                                  x * 32, y * 32);
                } else {
                    spriteManager.renderWallTile(WallSprites::EMPTY_CORRIDOR, 
                                                  x * 32, y * 32);
                }
            }
        }

        // Render Pac-Man
        spriteManager.renderPacMan(pacmanDirection, pacmanAnimFrame, pacmanX, pacmanY);

        // Render Ghosts
        spriteManager.renderGhost(GhostSprites::RED, ghostAnimFrame[0], ghostX[0], ghostY[0]);
        spriteManager.renderGhost(GhostSprites::BLUE, ghostAnimFrame[1], ghostX[1], ghostY[1]);
        spriteManager.renderGhost(GhostSprites::ORANGE, ghostAnimFrame[2], ghostX[2], ghostY[2]);
        spriteManager.renderGhost(GhostSprites::PINK, ghostAnimFrame[3], ghostX[3], ghostY[3]);

        // Render pellets
        for (int i = 0; i < 5; i++) {
            spriteManager.renderPellet(100 + i * 50, 400);
        }

        // Present
        SDL_RenderPresent(renderer);

        // Cap framerate at 60 FPS
        SDL_Delay(1000 / 60);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/