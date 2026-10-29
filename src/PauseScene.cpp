#include "PauseScene.h"
#include "MainMenu.h"
#include "Tetris.h"

using namespace TetrisConst;

void PauseScene::init(GameContext& context, Renderer& renderer){
    context.audio.pauseMusic();
    pauseFont = std::make_unique<Font>("assets/pong_font.ttf", 24);
    pauseTex = pauseFont->createTextTexture(renderer, "PAUSE", ColorCodes::WHITE);

}

void PauseScene::update(float deltaTime, GameContext& context){
    if(context.input.wasKeyPressed(Key::P)){
        context.audio.resumeMusic();
        context.scenes.popScene();
    }
    if(context.input.wasKeyPressed(Key::Escape)){
        context.scenes.changeScene(std::make_unique<MainMenu>());
    }
}

void PauseScene::render(Renderer& renderer){
    renderer.setColor(Color{128, 128, 128, 128});
    Rect fullScreen = {
        0.0f,
        0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT)
    };
    renderer.drawRect(fullScreen, true);

    renderer.drawTexture(pauseTex, Rect{(SCREEN_WIDTH - PAUSE_W)*0.5f, (SCREEN_HEIGHT - PAUSE_H)*0.5f, PAUSE_W, PAUSE_H});

}
