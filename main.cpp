#include "Engine.h"
#include "SDL2/SDL.h"
#include "Tetris.h"

using namespace TetrisConst;

int main(int argc, char* argv[]){
    char title[] = "Tetris";
    Engine engine(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO, SCREEN_WIDTH, SCREEN_HEIGHT, title);
    engine.setFPS(120.0);
    engine.setCursor(false);
    Tetris game;
    engine.run(game);
    return 0;
}
