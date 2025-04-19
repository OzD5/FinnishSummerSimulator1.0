#include "GameInitializer.h"

void GameInitializer::init(Game& game)
{
    initVariables(game);
    initWindow(game);
    initHand(game);
    initFonts(game);
    initUi(game);
    initGraphics(game);
    initSounds(game);
}
void GameInitializer::initVariables(Game& game)
{
    game.window = nullptr;
    game.widthRatio = static_cast<float>(game.windowWidth) / 1920.f;
    game.heightRatio = static_cast<float>(game.windowHeight) / 1080.f;

    // Game logic
    game.endGame = false;
    game.highscore = Save::getHighscore();
    game.points = 0;
    game.enemySpawnTimerMax = 200.f;

    if (game.difficulty == 4)
    {
        game.enemySpawnTimer = 1.f;
        game.enemySpawnTimerMax = 1.f;
        game.maxEnemies = 1000;
        game.health = 10000000;
    }
    else
    {
        game.enemySpawnTimer = game.enemySpawnTimerMax;
        game.maxEnemies = 7 * game.difficulty;
        game.health = 1100 - 100 * game.difficulty;
    }

    game.mouseHeld = false;
    game.isTouching = false;
    game.isMiss = false;
    game.isStaminaRegen = false;
    game.speedX = 5.f;
    game.speedY = 1.f * game.difficulty;
    game.mltplr = 1;
}

void GameInitializer::initWindow(Game& game)
{
    game.videoMode.height = game.windowHeight;
    game.videoMode.width = game.windowWidth;

    game.window = std::make_unique<sf::RenderWindow>(game.videoMode, "Finnish Summer Simulator", sf::Style::Titlebar | sf::Style::Close);
    game.window->setFramerateLimit(240);
}

void GameInitializer::initFonts(Game& game)
{
    if (!game.font.loadFromFile("Data/Fonts/UI_font.ttf"))
    {
        std::cout << "GAMEINITIALIZER::INITFONTS::FONT NOT LOADED";
    }
}

void GameInitializer::initUi(Game& game)
{
    //Scores etc. in uitext
    game.scoreText.setFont(game.font);
    game.scoreText.setCharacterSize(48);
    game.scoreText.setFillColor(sf::Color::White);
    game.scoreText.setOutlineColor(sf::Color::Black);
    game.scoreText.setOutlineThickness(0.5);
    game.scoreText.setString("NONE");
    game.scoreText.setScale(game.widthRatio, game.heightRatio);

    //Healthbar, Stamina Bar and their outlines

    initBar(game, game.healthBar, 20.f, 950.f, sf::Color::Green);
    initBar(game, game.staminaBar, 20.f, 900.f, sf::Color::Blue);

    initOutline(game, game.healthOutline, 20.f, 950.f);
    initOutline(game, game.staminaOutline, 20.f, 900.f);

    initText(game, game.scoreText, "NONE", 48, 5.f * game.widthRatio, 10.f * game.heightRatio);
    initText(game, game.healthText, "Health: ", 15,
        game.healthBar.getPosition().x + game.healthBar.getGlobalBounds().width + 20.f,
        game.healthBar.getPosition().y);
    initText(game, game.staminaText, "Stamina: ", 15,
        game.staminaBar.getPosition().x + game.staminaBar.getGlobalBounds().width + 20.f,
        game.staminaBar.getPosition().y);
}

void GameInitializer::initText(Game& game, sf::Text& textObj, std::string text, int size, float x, float y)
{
    textObj.setFont(game.font);
    textObj.setCharacterSize(size);
    textObj.setFillColor(sf::Color::White);
    textObj.setOutlineColor(sf::Color::Black);
    textObj.setOutlineThickness(0.5);
    textObj.setString(text);
    textObj.setPosition(x, y);
    textObj.setScale(game.widthRatio, game.heightRatio);
}

void GameInitializer::initBar(Game& game, sf::RectangleShape& bar, float x, float y, sf::Color color)
{
    bar.setSize(sf::Vector2f(200.f, 20.f));
    bar.setPosition(sf::Vector2f(game.widthRatio * x, game.heightRatio * y));
    bar.setFillColor(color);
    bar.setScale(game.widthRatio * 1.5f, game.heightRatio);
}

void GameInitializer::initOutline(Game& game, sf::RectangleShape& outline, float x, float y)
{
    outline.setSize(sf::Vector2f(200.f, 20.f));
    outline.setPosition(sf::Vector2f(game.widthRatio * x, game.heightRatio * y));
    outline.setFillColor(sf::Color(0, 0, 0, 0));
    outline.setOutlineColor(sf::Color::Black);
    outline.setOutlineThickness(-1.f);
    outline.setScale(game.widthRatio * 1.5f, game.heightRatio);
}

void GameInitializer::initGraphics(Game& game)
{
    if (!(game.bloodBath).loadFromFile("Data/Graphics/BloodBath.png"))
    {
        std::cout << "GAMEINITIALIZER::INITGRAPHICS::BLOODBATH.PNG NOT FOUND";
    }
    else
    {
        game.bloodBathObj.setTexture(game.bloodBath);
        game.bloodBathObj.setScale(2.0f * game.widthRatio, 2.0f * game.heightRatio);
    }
    if (!(game.backGround).loadFromFile("Data/Graphics/GameBG.png"))
    {
        std::cout << "GAMEINITIALIZER::INITGRAPHICS::Background.jpg NOT FOUND";
    }
    else
    {
        game.backGroundObj.setTexture(game.backGround);
        game.backGroundObj.setScale(1.3f * game.widthRatio, 1.1f * game.heightRatio);
        game.backGroundObj.setPosition(0, 0);
    }
    //Mosquito Pictures
    if (!game.mosquitoL.loadFromFile("Data/Graphics/mosquitoLL.png"))
    {
        std::cout << "GAMEINITIALIZER::INITGRAPHICS::MOSQUITOLL.PNG NOT FOUND";
    }
    if (!game.mosquitoR.loadFromFile("Data/Graphics/mosquitoRR.PNG"))
    {
        std::cout << "GAMEINITIALIZER::INITGRAPHICS::MOSQUITOR.PNG NOT FOUND";
    }
}

void GameInitializer::initSounds(Game& game)
{
    //Initting music
    std::string musicFile;
    switch (game.difficulty)
    {
    case 1:
        musicFile = "Data/Sounds/mainOST1.ogg";
        break;
    case 2:
        musicFile = "Data/Sounds/mainOST2.wav";
        break;
    case 3:
        musicFile = "Data/Sounds/mainOST2.2.wav";
        break;
    case 4:
        musicFile = "Data/Sounds/mainOST2.wav";
        break;
    default:
        musicFile = "Data/Sounds/mainOST2.wav";
        break;
    }

    if (!(game.musicOST).openFromFile(musicFile))
    {
        std::cout << "GAME::INITMUSIC::MAINOST1.OGG NOT FOUND";
    }
    game.musicOST.setLoop(true);
    game.musicOST.setVolume(20);
    game.musicOST.play();

    //Initting sound FX
    if (!(game.hittingHandFile).loadFromFile("Data/Sounds/HittingHand.ogg"))
    {
        std::cout << "GAMEINITIALIZER::INITMUSIC::HITTINGHAND.OGG NOT FOUND";
    }
    game.hittingHandSound.setBuffer(game.hittingHandFile);

    if (!(game.swingingAirFile).loadFromFile("Data/Sounds/SwingingAir.ogg"))
    {
        std::cout << "GAMEINITIALIZER::INITMUSIC::SWINGINGAIR.OGG NOT FOUND";
    }
    game.swingingAirSound.setBuffer(game.swingingAirFile);

    if (!(game.hittingInsectFile).loadFromFile("Data/Sounds/hittingInsect.ogg"))
    {
        std::cout << "GAMEINITIALIZER::INITMUSIC::HITTINGINSECT.OGG NOT FOUND";
    }
    game.hittingInsectSound.setBuffer(game.hittingInsectFile);
    game.hittingInsectSound.setVolume(50);

}

void GameInitializer::initHand(Game& game)
{
    //Hand object
    if (!game.handTexture.loadFromFile("Data/Graphics/HairyArm.png"))
    {
        std::cout << "GAMEINITIALIZER::INITHAND::HAIRYARM.PNG NOT FOUND";
    }
    game.hairyHand.setTexture(game.handTexture);
    game.hairyHand.setScale(sf::Vector2f(2.1f * game.widthRatio, 0.9f * game.heightRatio));
    game.hairyHand.setPosition(-10.f, 0.6f * game.windowHeight);
}
