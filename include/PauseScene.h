#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include <Game.h>


class PauseScene : public Game
{
    public:
        void init(GameContext& context, Renderer& renderer) override;
        void update(float deltaTime, GameContext& context) override;
        void render(Renderer& renderer) override;

    private:
        Texture pauseTex;
        std::unique_ptr<Font> pauseFont;
};

#endif // PAUSESCENE_H
