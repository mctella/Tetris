#ifndef TETRIS_H
#define TETRIS_H

#include <Game.h>

using namespace NoteCodes;

namespace TetrisConst{
    inline constexpr uint16_t SCREEN_WIDTH = 1920;
    inline constexpr uint16_t SCREEN_HEIGHT = 1080;
    inline constexpr uint16_t SIDE_PAD = SCREEN_WIDTH/5;

    const uint16_t TETROMINOES[7][4] = {
        {0x0F00, 0x2222, 0x00F0, 0x4444},   // I
        {0x44C0, 0x8E00, 0x6440, 0x0E20},   // J
        {0x4460, 0x0E80, 0xC440, 0x2E00},   // L
        {0xCC00, 0xCC00, 0xCC00, 0xCC00},   // O
        {0x06C0, 0x8C40, 0x6C00, 0x4620},   // S
        {0x0E40, 0x4C40, 0x4E00, 0x4640},   // T
        {0x0C60, 0x4C80, 0xC600, 0x2640}    // Z
    };

    const Color colors[8] = {
        ColorCodes::BLACK,
        ColorCodes::CYAN,
        ColorCodes::ORANGE,
        ColorCodes::BLUE,
        ColorCodes::PURPLE,
        ColorCodes::RED,
        ColorCodes::YELLOW,
        ColorCodes::GREEN
    };

    const uint8_t FRAMES[] = {
        48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
        5, 5, 5,
        4, 4, 4,
        3, 3, 3,
        2
    };

    const uint16_t POINTS[] = {40, 100, 300, 1200};

    constexpr uint8_t ROWS = 20;
    constexpr uint8_t COLS = 10;

    constexpr uint8_t BOARD_HEIGHT = 22;
    constexpr uint8_t BOARD_WIDTH = 12;

    inline constexpr float DAS_DELAY = 0.25f;
    inline constexpr float RAPID_REPEAT = 0.0f;

    inline constexpr float SPAWN_DELAY = 0.2f;

    inline constexpr uint8_t LINES = 10;
    inline constexpr uint8_t BOX_SIZE = SCREEN_HEIGHT/ROWS;

    inline constexpr int border = BOX_SIZE/8;

    const std::vector<Note> melodyAtrack1 = {
        {E4*2, 1.0f}, {B4, 0.5f}, {C4*2, 0.5f}, {D4*2, 1.0f}, {C4*2, 0.5f}, {B4, 0.5f},
        {A4, 1.0f}, {A4, 0.5f}, {C4*2, 0.5f}, {E4*2, 1.0f}, {D4*2, 0.5f}, {C4*2, 0.5f},
        {B4, 1.5f}, {C4*2, 0.5f}, {D4*2, 1.0f}, {E4*2, 1.0f}, {C4*2, 1.0f}, {A4, 1.0f},
        {A4, 2.0f}, {0.0f, 0.5f}, {D4*2, 1.0f}, {F4*2, 0.5f}, {A4*2, 1.0f}, {G4*2, 0.5f},
        {F4*2, 0.5f}, {E4*2, 1.0f}, {0.0f, 0.5f}, {C4*2, 0.5f}, {E4*2, 1.0f}, {D4*2, 0.5f}, {C4*2, 0.5f},
        {B4, 1.5f}, {C4*2, 0.5f}, {D4*2, 1.0f}, {E4*2, 1.0f}, {C4*2, 1.0f}, {A4, 1.0f},
        {A4, 2.0f},
    };
    const std::vector<Note> melodyAtrack2 = {
        {E4/4, 0.5f}, {E4/2, 0.5f}, {E4/4, 0.5f}, {E4/2, 0.5f}, {E4/4, 0.5f}, {E4/2, 0.5f},
        {E4/4, 0.5f}, {E4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f},
        {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {G4/2, 0.5f},
        {G4/2, 0.5f}, {D4/4, 0.5f}, {D4/2, 0.5}, {D4/4, 0.5f}, {D4/2, 0.5}, {D4/4, 0.5f}, {D4/2, 0.5},
        {D4/4, 0.5f}, {D4/2, 0.5}, {C4/4, 0.5f}, {C4/2, 0.5f}, {C4/4, 0.5f}, {C4/2, 0.5f},
        {C4/4, 0.5f}, {C4/2, 0.5f}, {C4/4, 0.5f}, {C4/2, 0.5f}, {B4/4, 0.5f}, {B4/2, 0.5f},
        {B4/4, 0.5f}, {B4/2, 0.5f}, {B4/4, 0.5f}, {B4/2, 0.5f}, {B4/4, 0.5f}, {B4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f},
    };
    const std::vector<Note> melodyAtrack3 = {
        {0.0, 1.0f}, {G4S, 0.5f}, {0.0, 1.5f}, {A4, 0.5f}, {G4, 0.5f}, {E4, 1.0f}, {0.0, 3.0f}, {G4S, 1.0f},
        {G4S, 0.5f}, {A4, 0.5f}, {B4, 1.0f}, {0.0, 5.5f}, {F4, 1.0f}, {A4, 0.5f}, {C4, 1.0f},
        {B4, 0.5f}, {A4, 0.5f}, {G4, 1.0f}, {0.0, 0.5f}, {E4, 0.5f}, {G4, 1.0f}, {F4, 0.5f}, {E4, 0.5f},
        {G4S, 1.0f}, {G4S, 0.5f}, {A4, 0.5f}, {B4, 1.0f}, {0.0, 5.0f}
    };

    const std::vector<Note> melodyBtrack1 = {
        {E4, 2.0f}, {C4, 2.0f}, {D4, 2.0f}, {B4/2, 2.0f}, {C4, 2.0f}, {A4/2, 2.0f}, {G4S/2, 2.0f},
        {B4/2, 2.0f}, {E4, 2.0f}, {C4, 2.0f}, {D4, 2.0f}, {B4/2, 2.0f},{C4, 1.0f}, {E4, 1.0f}, {A4, 2.0f},
        {G4S, 4.0f}
    };

    const std::vector<Note> melodyBtrack2 = {
        {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f},
        {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f},
        {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f}, {A4/4, 0.5f}, {A4/2, 0.5f},
        {A4/4, 0.5f}, {A4/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f},
        {G4S/4, 0.5f}, {G4S/2, 0.5f}, {G4S/4, 0.5f}, {G4S/2, 0.5f}, {A4/4, 0.5f}, {E4/4, 0.5f},
        {A4/4, 0.5f}, {A4, 0.5f}, {A4/4, 0.5f}, {A4, 0.5f}, {A4/4, 0.5f}, {A4, 0.5f}, {E4/4, 0.5f}, {E4/2, 0.5f}, {E4/4, 0.5f}, {E4/2, 0.5f}, {E4/4, 0.5f}, {E4/2, 0.5f},
        {E4/4, 0.5f}, {E4/2, 0.5f}
    };

    const std::vector<Note> melodyBtrack3 = {
        {C4, 2.0f}, {A4/2, 2.0f}, {B4/2, 2.0f}, {G4S/2, 2.0f}, {A4/2, 2.0f}, {E4/2, 2.0f}, {E4/2, 2.0f},
        {G4S/2, 2.0f}, {C4, 2.0f}, {A4/2, 2.0f}, {G4S/2, 2.0f}, {B4/2, 2.0f},{G4S/2, 1.0f}, {C4, 1.0f}, {E4, 2.0f},
        {E4, 4.0f}
    };

    inline constexpr int8_t KICK_TABLE_I[4][2][5][2] = {
        // State 0 (Spawn)
        {
            {{ 0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}}, // 0 -> 3 (Left)
            {{ 0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}}  // 0 -> 1 (Right)
        },
        // State 1 (Rotated Right)
        {
            {{ 0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}}, // 1 -> 0 (Left)
            {{ 0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}}  // 1 -> 2 (Right)
        },
        // State 2 (Upside Down)
        {
            {{ 0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}}, // 2 -> 1 (Left)
            {{ 0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}}  // 2 -> 3 (Right)
        },
        // State 3 (Rotated Left)
        {
            {{ 0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}}, // 3 -> 2 (Left)
            {{ 0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}}  // 3 -> 0 (Right)
        }
    };

    inline constexpr int8_t KICK_TABLE[4][2][5][2] = {
        // State 0 (Spawn)
        {
            {{ 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}}, // 0 -> 3 (Left)
            {{ 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}}  // 0 -> 1 (Right)
        },
        // State 1 (Rotated Right)
        {
            {{ 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}}, // 1 -> 0 (Left)
            {{ 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}}  // 1 -> 2 (Right)
        },
        // State 2 (Upside Down)
        {
            {{ 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}}, // 2 -> 1 (Left)
            {{ 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}}  // 2 -> 3 (Right)
        },
        // State 3 (Rotated Left)
        {
            {{ 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}}, // 3 -> 2 (Left)
            {{ 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}}  // 3 -> 0 (Right)
        }
    };
};

class Tetris : public Game
{
    public:
        void init(GameContext& context, Renderer& rendere) override;

        void update(float deltaTime, GameContext& context) override;

        void render(Renderer& renderer) override;

        void moveRight(float timer, AudioSystem& audio);
        void moveLeft(float timer, AudioSystem& audio);
        void moveDown(float timer);

        void setPiece();

        void rotateSRS(bool clockwise, AudioSystem& audio);

        void renderCurrPiece(Renderer& renderer, Texture& blockTex);
        void renderBoard(Renderer& renderer);

        bool checkCollision();
        void fix();
        void newPiece();

        bool checkLine(uint8_t line);
        void deleteLine(uint8_t line);

        void setDropSpeedMS();

        void increaseScore(uint8_t numLines);

        void levelUp();

        void fillBag();

        SoundBuffer buildFullTheme(AudioSystem& audio);
        Texture createBlockTexture(Renderer& renderer, uint8_t rot);
    private:
        uint16_t currPiece;
        uint8_t pieceType;
        int8_t r;
        int8_t c;
        uint8_t currRot;

        float dasTimer[3];
        float currGravitySpeed;
        float gravityTimer;

        float spawnDelayTimer;

        uint8_t level;
        int score;

        uint8_t lines;

        uint8_t board[TetrisConst::BOARD_HEIGHT][TetrisConst::BOARD_WIDTH] = {
            {9, 0,0,0,0,0,0,0,0,0,0, 9}, // Row 0 (Top)
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9},
            {9, 0,0,0,0,0,0,0,0,0,0, 9}, // Row 20
            {9, 9,9,9,9,9,9,9,9,9,9, 9}  // Row 21 (Floor)
        };

        Rect boardRect;
        Rect scoreRect;

        std::vector<uint8_t> bag;
        uint8_t bagIndex = 0;

        SoundBuffer song;
        SoundBuffer rotateSfx;
        SoundBuffer moveSfx;
        SoundBuffer fixSfx;
        SoundBuffer clearSfx;
        SoundBuffer tetrisSfx;

        Texture blocktextures[7];
};

#endif // TETRIS_H
