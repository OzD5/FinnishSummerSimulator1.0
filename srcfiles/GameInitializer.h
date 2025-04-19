#pragma once

#include "Game.h"
#include "Save.h"

class GameInitializer
{
public:
    static void init(Game& game);

private:
    static void initVariables(Game& game);
    static void initWindow(Game& game);
    static void initFonts(Game& game);
    static void initUi(Game& game);
    static void initText(Game& game, sf::Text& textObj, std::string text, int size, float x, float y);
    static void initBar(Game& game, sf::RectangleShape& bar, float x, float y, sf::Color color);
    static void initOutline(Game& game, sf::RectangleShape& outline, float x, float y);
    static void initGraphics(Game& game);
    static void initSounds(Game& game);
    static void initHand(Game& game);

};
