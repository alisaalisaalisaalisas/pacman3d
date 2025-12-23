#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

// Sprite coordinates based on SETUP_GUIDE.md

namespace PacManSprites {
    constexpr int FRAME_SIZE = 32;
    constexpr int SHEET_WIDTH = 384;
    constexpr int SHEET_HEIGHT = 128;
    
    constexpr int UP = 0;
    constexpr int DOWN = 1;
    constexpr int LEFT = 2;
    constexpr int RIGHT = 3;
    
    constexpr int FRAMES_X[4][3] = {
        {0, 32, 64},
        {96, 128, 160},
        {192, 224, 256},
        {288, 320, 352}
    };
    constexpr int FRAMES_Y = 0;
}

namespace GhostSprites {
    constexpr int FRAME_SIZE = 32;
    constexpr int SHEET_WIDTH = 256;
    constexpr int SHEET_HEIGHT = 64;
    
    constexpr int RED = 0;
    constexpr int BLUE = 1;
    constexpr int ORANGE = 2;
    constexpr int PINK = 3;
    
    constexpr int FRAMES_X[4][2] = {
        {0, 32},
        {64, 96},
        {128, 160},
        {192, 224}
    };
    constexpr int FRAMES_Y = 0;
}

namespace CollectibleSprites {
    constexpr int PELLET_X = 0;
    constexpr int PELLET_Y = 12;
    constexpr int PELLET_SIZE = 8;
    constexpr int POWER_X[3] = {32, 64, 96};
    constexpr int POWER_Y = 0;
    constexpr int POWER_SIZE = 32;
}

#endif // SPRITE_DATA_H