#include "Tetris.h"

#include <fstream>
#include <ios>

using namespace TetrisConst;
using namespace NoteCodes;

void Tetris::init(GameContext& context, Renderer& renderer){
    SongData themeData = context.resources.readSongFromFile("assets/tetris_theme.trk");
    song = context.audio.buildFullTheme(themeData);
    context.audio.playMusic(song);

    createBorderTexture(renderer);
    for(uint8_t i = 0; i < 7; i++){
        blocktextures[i] = renderer.makeTextureFromMap8(&blockTile[0][0][0], border, &colors[i+1], BOX_SIZE, BOX_SIZE, 8);
    }

    rotateSfx = context.audio.generateTone(G4, 0.5f, 0.1f);
    moveSfx = context.audio.generateTone(G4S*2, 0.3f, 0.08f);;

    std::vector<Note> fixNotes = {
        {C4 / 2.0f, 0.125f}, {G4 / 4.0f, 0.125f}
    };
    fixSfx = context.audio.generateSong(fixNotes, 70.0f, 0.5f);

    std::vector<Note> clearNotes = {
        {G4 * 2.0f, 0.125f}, {C4 * 4.0f, 0.25f}
    };
    clearSfx = context.audio.generateSong(clearNotes, 70.0f, 0.5f);

    std::vector<Note> tetrisNotes = {
        {C4 * 2.0f, 0.125f}, {E4 * 2.0f, 0.125f}, {G4 * 2.0f, 0.125f}, {C4 * 4.0f, 0.5f}
    };
    tetrisSfx = context.audio.generateSong(tetrisNotes, 70.0f, 0.5f);

    std::vector<Note> levelupNotes = {
        {C4 * 2.0f, 0.25f}, {G4, 0.25f}, {C4 * 2.0f, 0.25f}, {E4 * 2.0f, 0.25f}, {G4 * 2.0f, 0.5f}
    };
    levelupSfx = context.audio.generateSong(levelupNotes, 70.0f, 0.5f);

    boardRect = {
        (SCREEN_WIDTH/2) - (BOX_SIZE*5),
        PADDING,
        BOX_SIZE*10,
        BOX_SIZE*20
    };

    scoreRect = {
        0,
        0,
        SCREEN_WIDTH/5,
        64
    };

    borderRect = {
        boardRect.x - PADDING,
        0,
        BOX_SIZE * 10 + PADDING*2,
        SCREEN_HEIGHT
    };

    isGridOn = false;

    dasTimer[0] = 0.0f;
    dasTimer[1] = 0.0f;
    dasTimer[2] = 0.0f;

    level = 0;
    score = 0;
    lines = LINES;

    setDropSpeedMS();

    fillBag();
    pieceType = bag.at(bagIndex++);
    nextPieceType = bag.at(bagIndex++);
    newPiece();
}

void Tetris::update(float deltaTime, GameContext& context){
    if(spawnDelayTimer > 0.0f){
        spawnDelayTimer -= deltaTime;
        return;
    }

    if(context.input.wasKeyPressed(Key::Q)){
        rotateSRS(false, context.audio);
    }
    if(context.input.wasKeyPressed(Key::W)){
        while(!checkCollision()){
            r++;
        }
        r--;
        fix(context.audio);
    }
    if(context.input.wasKeyPressed(Key::E)){
        rotateSRS(true, context.audio);
    }
    if(context.input.wasKeyPressed(Key::D)){
        moveRight(DAS_DELAY, context.audio);
    }else if(context.input.isKeyDown(Key::D)){
        dasTimer[2] -= deltaTime;
        if(dasTimer[2] <= 0.0f){
            moveRight(RAPID_REPEAT, context.audio);
        }
    }
    if(context.input.wasKeyPressed(Key::S)){
        moveDown(DAS_DELAY, context.audio);
        gravityTimer = currGravitySpeed;
    }else if(context.input.isKeyDown(Key::S)){
        dasTimer[1] -= deltaTime;
        if(dasTimer[1] <= 0.0f){
            moveDown(RAPID_REPEAT, context.audio);
            gravityTimer = currGravitySpeed;
        }
    }
    if(context.input.wasKeyPressed(Key::A)){
        moveLeft(DAS_DELAY, context.audio);
    }else if(context.input.isKeyDown(Key::A)){
        dasTimer[0] -= deltaTime;
        if(dasTimer[0] <= 0.0f){
            moveLeft(RAPID_REPEAT, context.audio);
        }
    }
    if(context.input.wasKeyPressed(Key::G)){
        isGridOn = (isGridOn) ? false : true;
    }
    if(context.input.isKeyDown(Key::Escape)){
        context.input.setQuit(true);
    }
    if(context.input.wasKeyPressed(Key::LeftArr)){
        rotateSRS(false, context.audio);
    }
    if(context.input.wasKeyPressed(Key::RightArr)){
        rotateSRS(true, context.audio);
    }

    gravityTimer -= deltaTime;
    if(gravityTimer <= 0.0f){
        moveDown(0.0f, context.audio);
        gravityTimer = currGravitySpeed;
    }
}

void Tetris::render(Renderer& renderer){
    renderer.setColor(ColorCodes::BLACK);
    renderer.clear();

    renderer.setColor(ColorCodes::WHITE);
    renderer.drawScore(scoreRect, score, 6);

    renderer.drawTexture(borderTexture[level % 3], borderRect);
    renderBoard(renderer);
    renderCurrPiece(renderer, blocktextures[pieceType]);
}

void Tetris::moveRight(float timer, AudioSystem& audio){
    dasTimer[2] = timer;
    c++;
    if(checkCollision()){
        c--;
        return;
    }
    audio.playSfx(moveSfx);
}

void Tetris::moveLeft(float timer, AudioSystem& audio){
    dasTimer[0] = timer;
    c--;
    if(checkCollision()){
        c++;
        return;
    }
    audio.playSfx(moveSfx);
}

void Tetris::moveDown(float timer, AudioSystem& audio){
    dasTimer[1] = timer;
    r++;
    if(checkCollision()){
        r--;
        fix(audio);
    }
}

void Tetris::setPiece(){
    pieceType = (pieceType % 7);
    currPiece = TETROMINOES[pieceType][currRot];
}

void Tetris::rotateSRS(bool clockwise, AudioSystem& audio){
    uint8_t oldRot = currRot;

    uint8_t nextRot = clockwise ? (currRot + 1) % 4 : (currRot + 3) % 4;
    uint8_t dirIndex = clockwise ? 1 : 0;

    if(pieceType == 3){
        currRot = nextRot;
        setPiece();
        if(checkCollision()){
            currRot = clockwise ? (nextRot - 1 + 4) % 4 : (nextRot + 1) % 4;
            setPiece();
            audio.playSfx(rotateSfx);
        }
        return;
    }

    for(uint8_t test = 0; test < 5; test++){
        int8_t offsetX;
        int8_t offsetY;

        if(!pieceType){
            offsetX = KICK_TABLE_I[oldRot][dirIndex][test][0];
            offsetY = KICK_TABLE_I[oldRot][dirIndex][test][1];
        }else{
            offsetX = KICK_TABLE[oldRot][dirIndex][test][0];
            offsetY = KICK_TABLE[oldRot][dirIndex][test][1];
        }


        c += offsetX;
        r -= offsetY;
        currRot = nextRot;
        setPiece();

        if(!checkCollision()){
            audio.playSfx(rotateSfx);
            return;
        }

        c -= offsetX;
        r += offsetY;
    }

    currRot = oldRot;
    setPiece();
}

/*
Texture Tetris::createBlockTexture(Renderer& renderer, uint8_t rot){

    Texture blockTex = renderer.createRenderTarget(BOX_SIZE, BOX_SIZE);
    renderer.setRenderTarget(&blockTex);

    renderer.setColor(colors[rot]);
    renderer.drawRect({0, 0, BOX_SIZE, BOX_SIZE}, 1);

    uint8_t prevShade = 0;
    Rect pixel = {
        0,
        0,
        border,
        border
    };

    for(uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; j < 8; j++){
            if(blockTile[i][j] != 128){
                if(blockTile[i][j] != prevShade){
                    renderer.setColor(calculateShade(colors[rot], blockTile[i][j]));
                    prevShade = blockTile[i][j];
                }
                renderer.drawRect(pixel, true);
            }
            pixel.x += border;
        }
        pixel.x = 0;
        pixel.y += border;
    }

    // Reset rendering back to the main screen
    renderer.setRenderTarget(nullptr);
    return blockTex;

}*/


void Tetris::createBorderTexture(Renderer& renderer){
    Color cols[3] = { ColorCodes::BRONZE, ColorCodes::SILVER, ColorCodes::GOLD };
    float w = (BOX_SIZE*10 + PADDING*2);
    float nb = border*2;
    for(uint8_t i = 0; i < 3; i++){
        borderTexture[i] = renderer.createRenderTarget(w, SCREEN_HEIGHT);
        renderer.setRenderTarget(&borderTexture[i]);

        renderer.setColor(cols[i]);
        renderer.drawRect({0, 0, PADDING, SCREEN_HEIGHT}, true);            // LEFT
        renderer.drawRect({0, 0, w, PADDING}, true);                        // TOP
        renderer.drawRect({w - PADDING, 0, PADDING, SCREEN_HEIGHT}, true);  // RIGHT
        renderer.drawRect({0, BOX_SIZE*20 + PADDING, w, PADDING}, true);    // BOTTOM

        renderer.setColor(calculateHighlight(cols[i]));
        renderer.drawRect({0, 0, nb, SCREEN_HEIGHT}, true);            // LEFT
        renderer.drawRect({0, 0, w, nb}, true);                        // TOP
        renderer.drawRect({w - PADDING, PADDING - nb, nb, SCREEN_HEIGHT - PADDING*2 + nb}, true);  // RIGHT
        renderer.drawRect({PADDING - nb, BOX_SIZE*20 + PADDING, BOX_SIZE*10 + nb*2, nb}, true);    // BOTTOM

        renderer.setColor(calculateShadow(cols[i]));
        renderer.drawRect({PADDING - nb, PADDING, nb, SCREEN_HEIGHT - PADDING*2}, true);            // LEFT
        renderer.drawRect({PADDING - nb, PADDING - nb, BOX_SIZE*10 + nb*2, nb}, true);                        // TOP
        renderer.drawRect({w - nb, 0, nb, SCREEN_HEIGHT}, true);  // RIGHT
        renderer.drawRect({0, SCREEN_HEIGHT - nb, w, nb}, true);    // BOTTOM
    }

    renderer.setRenderTarget(nullptr);
}

void Tetris::renderCurrPiece(Renderer& renderer, Texture& blockTex){
    int startX = boardRect.x + (BOX_SIZE*(c-1));
    int startY = boardRect.y + (BOX_SIZE*(r-1));

    uint16_t p = currPiece;

    for(int pos = 0; p > 0; pos++, p <<=1){
        if(p & 0x8000){
            int x = pos%4;
            int y = pos/4;

            Rect dest = {
                static_cast<float>(startX + (x*BOX_SIZE)),
                static_cast<float>(startY + (y*BOX_SIZE)),
                BOX_SIZE,
                BOX_SIZE
            };

            renderer.drawTexture(blockTex, dest);
        }
    }
}

void Tetris::renderBoard(Renderer& renderer){
    if(isGridOn){
        renderer.setColor(ColorCodes::WHITE);
        renderer.drawGrid(boardRect, ROWS, COLS);
    }

    for(int i = 1; i < BOARD_HEIGHT - 1 ; i++){
        for(int j = 1; j < BOARD_WIDTH - 1; j++){
            int blockType = board[i][j];
            if(blockType > 0){
                Rect cell = {boardRect.x + ((j-1)*BOX_SIZE), boardRect.y + ((i-1)*BOX_SIZE), BOX_SIZE, BOX_SIZE};
                renderer.drawTexture(blocktextures[blockType-1], cell);
            }
        }
    }
}

bool Tetris::checkCollision() const{
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            if((currPiece >> (15 - ((row*4)+col))) & 1){
                int8_t checkRow = r + row;
                int8_t checkCol = c + col;

                if(checkRow < 0 || checkRow >= BOARD_HEIGHT ||
                   checkCol < 0 || checkCol >= BOARD_WIDTH){
                    return true;
                }

                if(board[r + row][c + col]) return true;
            }
        }
    }
    return false;
}

void Tetris::fix(AudioSystem& audio){
    for(uint8_t row = 0; row < 4; row++){
        for(uint8_t col = 0; col < 4; col++){
            if((currPiece >> (15 - ((row*4)+col))) & 1){
                uint8_t boardRow = r + row;
                uint8_t boardCol = c + col;

                if(boardRow >= 0 && boardRow < BOARD_HEIGHT &&
                   boardCol >= 0 && boardCol < BOARD_WIDTH){
                    board[r+row][c+col] = pieceType + 1;
                }
            }
        }
    }
    uint8_t deletedCOunt = 0;
    for(uint8_t row = 0; row < 4; row++){
        uint8_t boardRow = r + row;
        if(boardRow >= BOARD_HEIGHT - 1) continue;
        if(checkLine(boardRow)){
            deleteLine(boardRow, audio);
            deletedCOunt++;
        }
    }

    if(deletedCOunt){
        increaseScore(deletedCOunt);
        if(deletedCOunt == 4){
            audio.playSfx(tetrisSfx);
        }else{
            audio.playSfx(clearSfx);
        }
    }else{
        audio.playSfx(fixSfx);
    }

    newPiece();
}

bool Tetris::checkLine(uint8_t line) const{
    for(uint8_t i = 1; i < BOARD_WIDTH - 1; i++){
        if(!board[line][i]) return false;
    }
    return true;
}

void Tetris::deleteLine(uint8_t line, AudioSystem& audio){
    size_t bytesToMove = (line - 1) * BOARD_WIDTH;
    memmove(&board[2][0], &board[1][0], bytesToMove);
    for(int i = 1; i < BOARD_WIDTH - 1; i++){
        board[1][i] = 0;
    }
    lines--;
    if(!lines){
        levelUp(audio);
    }
}

void Tetris::newPiece(){
    spawnDelayTimer = SPAWN_DELAY;
    dasTimer[1] = DAS_DELAY;

    pieceType = nextPieceType;

    if(bagIndex >= 7){
        fillBag();
    }

    nextPieceType = bag.at(bagIndex);
    bagIndex++;

    r = 1;
    c = 4;
    currRot = 0;
    setPiece();

    if(checkCollision()){
        exit(EXIT_FAILURE);
    }
}

void Tetris::setDropSpeedMS(){
    uint8_t frame = 2;
    if(level >= 29){
        frame = 1;
    }else if(level < 20){
        frame = FRAMES[level];
    }

    float sPerFrame = 1.0f / 60.0f;
    currGravitySpeed = static_cast<float>(frame)*sPerFrame;
    gravityTimer = currGravitySpeed;
}

void Tetris::increaseScore(uint8_t lines){
    score += (POINTS[lines+1]) * (level+1);
}

void Tetris::levelUp(AudioSystem& audio){
    lines = LINES;
    level++;
    setDropSpeedMS();
    audio.playSfx(levelupSfx);
}

void Tetris::fillBag(){
    std::iota(bag.begin(), bag.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bag.begin(), bag.end(), g);

    bagIndex = 0;
}

