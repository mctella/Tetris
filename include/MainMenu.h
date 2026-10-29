#ifndef MAINMENU_H
#define MAINMENU_H

#pragma once
#include <Game.h>
#include "Font.h"
#include "Tetris.h"


class MainMenu : public Game
{
    private:
        Texture titleLetters;
        Texture startPromptTex;

        float titleStartX = 0.0f;
        float titleStartY = 0.0f;
        float promptX = 0.0f;
        float promptY = 0.0f;

        const float LETTER_SPACING = 12.0f;

    public:
        void init(GameContext& context, Renderer& renderer) override;
        void update(float deltaTime, GameContext& context) override;
        void render(Renderer& renderer) override;
};

#endif // MAINMENU_H
