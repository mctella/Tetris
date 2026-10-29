#include "MainMenu.h"

void MainMenu::init(GameContext& context, Renderer& renderer){
    context.audio.stopMusic();

    Font titleFont("assets/pong_font.ttf", 96);
    Font promptFont("assets/pong_font.ttf", 28);

    std::string titleText = "TETRIS";

    Color letterColor[6] = {
        ColorCodes::RED,
        ColorCodes::ORANGE,
        ColorCodes::YELLOW,
        ColorCodes::GREEN,
        ColorCodes::CYAN,
        ColorCodes::VIOLET
    };

    std::vector<Texture> tempLetters;

    float totalTitleWidth = 0.0f;
    float maxHeight = 0.0f;

    for(size_t i = 0; i < titleText.length(); i++){
        std::string charStr(1, titleText[i]);

        Color col = letterColor[i % 6];

        Texture letterTex = titleFont.createTextTexture(renderer, charStr, col);
        totalTitleWidth += letterTex.getWidth();
        if(letterTex.getHeight() > maxHeight) maxHeight = letterTex.getHeight();

        tempLetters.push_back(std::move(letterTex));
    }

    totalTitleWidth += LETTER_SPACING * (titleText.length() - 1);

    titleLetters = renderer.createRenderTarget(static_cast<int>(totalTitleWidth), static_cast<int>(maxHeight));
    renderer.setRenderTarget(&titleLetters);

    renderer.setColor(0, 0, 0, 0);
    renderer.clear();

    float currentX = 0.0f;
    for(const auto& letterTex : tempLetters){
        Rect dst = {
            currentX,
            0.0f,
            static_cast<float>(letterTex.getWidth()),
            static_cast<float>(letterTex.getHeight())
        };

        renderer.drawTexture(letterTex, dst);
        currentX += letterTex.getWidth() + LETTER_SPACING;
    }

    renderer.setRenderTarget(nullptr);

    titleStartX = (TetrisConst::SCREEN_WIDTH - totalTitleWidth)*0.5f;
    titleStartY = 300.0f;

    startPromptTex = promptFont.createTextTexture(renderer, "PRESS SPACE", ColorCodes::WHITE);
    promptX = (TetrisConst::SCREEN_WIDTH - startPromptTex.getWidth())*0.5f;
    promptY = 800.0f;
}

void MainMenu::update(float deltaTime, GameContext& context){
    if(context.input.wasKeyPressed(Key::Space)){
        context.scenes.changeScene(std::make_unique<Tetris>());
    }
    if(context.input.wasKeyPressed(Key::Escape)){
        context.input.setQuit(true);
    }
}

void MainMenu::render(Renderer& renderer){
    renderer.setColor(ColorCodes::BLACK);
    renderer.clear();

    renderer.drawTexture(titleLetters, {titleStartX, titleStartY, static_cast<float>(titleLetters.getWidth()), static_cast<float>(titleLetters.getHeight())});
    renderer.drawTexture(startPromptTex, {promptX, promptY, static_cast<float>(startPromptTex.getWidth()), static_cast<float>(startPromptTex.getHeight())});

    if(startPromptTex.get() != nullptr){
        Rect promptDst = {
            promptX,
            promptY,
            static_cast<float>(startPromptTex.getWidth()),
            static_cast<float>(startPromptTex.getHeight())
        };
        renderer.drawTexture(startPromptTex, promptDst);
    }
}
