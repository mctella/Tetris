#include "Tetris.h"

using namespace TetrisConst;

void Tetris::init(GameContext& context, Renderer& renderer){
    song = buildFullTheme(context.audio);
    context.audio.playMusic(song);

    for(uint8_t i = 0; i < 7; i++){
        blocktextures[i] = createBlockTexture(renderer, i+1);
    }

    rotateSfx = context.audio.generateTone(G4, 0.2f, 0.1f);
    moveSfx = context.audio.generateTone(G4S*2, 0.2f, 0.1f);;
    fixSfx = context.audio.generateTone(G4/2, 0.2f, 0.1f);
    clearSfx = context.audio.generateTone(E4, 0.2f, 0.1f);;
    //tetrisSfx;

    boardRect = {
        (SCREEN_WIDTH/2) - (BOX_SIZE*5),
        0,
        BOX_SIZE*10,
        BOX_SIZE*20
    };

    scoreRect = {
        0,
        0,
        120,
        20
    };

    dasTimer[0] = 0.0f;
    dasTimer[1] = 0.0f;
    dasTimer[2] = 0.0f;

    level = 0;
    score = 0;
    lines = LINES;

    setDropSpeedMS();
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
        fix();
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
        moveDown(DAS_DELAY);
        gravityTimer = currGravitySpeed;
    }else if(context.input.isKeyDown(Key::S)){
        dasTimer[1] -= deltaTime;
        if(dasTimer[1] <= 0.0f){
            moveDown(RAPID_REPEAT);
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
    if(context.input.isKeyDown(Key::Escape)){
        context.input.setQuit(true);
    }

    gravityTimer -= deltaTime;
    if(gravityTimer <= 0.0f){
        moveDown(0.0f);
        gravityTimer = currGravitySpeed;
    }
}

SoundBuffer Tetris::buildFullTheme(AudioSystem& audio){
    SoundBuffer mAt1 = audio.generateSong(melodyAtrack1, 140.0f, 0.5f);
    SoundBuffer mAt2 = audio.generateSong(melodyAtrack2, 140.0f, 0.5f);
    SoundBuffer mAt3 = audio.generateSong(melodyAtrack3, 140.0f, 0.2f);
    SoundBuffer pA = audio.mixTracks(mAt1, mAt2);
    SoundBuffer partA = audio.mixTracks(pA, mAt3);
    SoundBuffer mBt1 = audio.generateSong(melodyBtrack1, 140.0f, 0.5f);
    SoundBuffer mBt2 = audio.generateSong(melodyBtrack2, 140.0f, 0.5f);
    SoundBuffer pB = audio.mixTracks(mBt1, mBt2);
    SoundBuffer mBt3 = audio.generateSong(melodyBtrack3, 140.0f, 0.2f);
    SoundBuffer partB = audio.mixTracks(pB, mBt3);

    SoundBuffer fullSong;

    auto append = [&](const SoundBuffer& part){
        fullSong.samples.insert(
            fullSong.samples.end(),
            part.samples.begin(),
            part.samples.end()
        );
    };

    append(partA);
    append(partA);
    append(partB);
    append(partB);

    return fullSong;
}

void Tetris::render(Renderer& renderer){
    renderer.setColor(ColorCodes::BLACK);
    renderer.clear();

    renderer.setColor(ColorCodes::WHITE);
    renderer.drawScore(scoreRect, score, 6);

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
    }
    audio.playSfx(moveSfx);
}

void Tetris::moveDown(float timer){
    dasTimer[1] = timer;
    r++;
    if(checkCollision()){
        r--;
        fix();
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

Texture Tetris::createBlockTexture(Renderer& renderer, uint8_t rot){
    Texture blockTex = renderer.createRenderTarget(BOX_SIZE, BOX_SIZE);
    renderer.setRenderTarget(&blockTex);

    renderer.setColor(colors[rot]);
    renderer.drawRect({0, 0, BOX_SIZE, BOX_SIZE}, 1);

    renderer.setColor(ColorCodes::WHITE);
    renderer.drawRect({0, 0, BOX_SIZE, border}, 1);
    renderer.drawRect({0, 0, border, BOX_SIZE}, 1);

    renderer.setColor(ColorCodes::BLACK);
    renderer.drawRect({0, BOX_SIZE - border, BOX_SIZE, border}, 1);
    renderer.drawRect({BOX_SIZE - border, 0, border, BOX_SIZE}, 1);

    // Reset rendering back to the main screen
    renderer.setRenderTarget(nullptr);
    return blockTex;
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
                startX + (x*BOX_SIZE),
                startY + (y*BOX_SIZE),
                BOX_SIZE,
                BOX_SIZE
            };

            renderer.drawTexture(blockTex, dest);
        }
    }

    /*
    Rect box = {
        boardRect.x + (BOX_SIZE*(c-1)),
        boardRect.y + (BOX_SIZE*(r-1)),
        BOX_SIZE,
        BOX_SIZE
    };

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int bitIdx = 15 - ((i*4)+j);
            if((currPiece >> bitIdx) & 1){
                Rect cell = {box.x + j*box.width, box.y + i*box.height, box.width, box.height};

                renderer.setColor(colors[pieceType+1]);
                renderer.drawRect(cell, 1);

                renderer.setColor(ColorCodes::WHITE);
                Rect topEdge = {cell.x, cell.y, cell.width, border};
                Rect leftEdge = {cell.x, cell.y, border, cell.height};
                renderer.drawRect(topEdge, 1);
                renderer.drawRect(leftEdge, 1);

                // 3. Draw the Shadow (Bottom and Right edges)
                renderer.setColor(ColorCodes::BLACK);
                Rect bottomEdge = {cell.x, cell.y + cell.height - border, cell.width, border};
                Rect rightEdge = {cell.x + cell.width - border, cell.y, border, cell.height};
                renderer.drawRect(bottomEdge, 1);
                renderer.drawRect(rightEdge, 1);

                renderer.setColor(ColorCodes::WHITE);
                Rect innerDetail = {cell.x + 4, cell.y + 4, border * 2, border * 2};
                renderer.drawRect(innerDetail, 1);
            }
        }
    }
    */
}

void Tetris::renderBoard(Renderer& renderer){
    renderer.setColor(ColorCodes::WHITE);
    renderer.drawGrid(boardRect, ROWS, COLS);

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

bool Tetris::checkCollision(){
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

void Tetris::fix(){
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
            deleteLine(boardRow);
            deletedCOunt++;
        }
    }
    if(deletedCOunt)    increaseScore(deletedCOunt);
    newPiece();
}

bool Tetris::checkLine(uint8_t line){
    for(uint8_t i = 1; i < BOARD_WIDTH - 1; i++){
        if(!board[line][i]) return false;
    }
    return true;
}

void Tetris::deleteLine(uint8_t line){
    size_t bytesToMove = (line - 1) * BOARD_WIDTH;
    memmove(board[2], board[1], bytesToMove);
    for(int i = 1; i < BOARD_WIDTH - 1; i++){
        board[1][i] = 0;
    }
    lines--;
    if(!lines){
        levelUp();
    }
}

void Tetris::newPiece(){
    spawnDelayTimer = SPAWN_DELAY;
    dasTimer[1] = DAS_DELAY;

    if(bag.empty() || bagIndex >= 7){
        fillBag();
    }

    r = 1;
    c = 4;
    currRot = 0;
    pieceType = bag.at(bagIndex);
    bagIndex++;
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

void Tetris::levelUp(){
    lines = LINES;
    level++;
    setDropSpeedMS();
}

void Tetris::fillBag(){
    bag = {0, 1, 2, 3, 4, 5, 6};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bag.begin(), bag.end(), g);

    bagIndex = 0;
}
