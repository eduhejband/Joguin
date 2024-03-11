
#ifndef GAME2_H
#define GAME2_H

#include <SFML/Graphics.hpp>

class Game2 {
public:
    Game2();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void resetGame();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void handleBarCollision(const sf::RectangleShape& bar, bool isBottomBar);

    sf::RenderWindow window;
    sf::RectangleShape energyBar;
    sf::RectangleShape topEnergyBar;
    sf::CircleShape ball;
    sf::Vector2f ballVelocity;
    sf::Text returnText;

    float moveSpeed;
    bool followBall;
    sf::Clock clock;
    sf::Clock ballStartTimer;
    bool gameActive;
    bool showRetryButton;
    std::string winnerTextString;
    sf::Font font;
    sf::Text retryText;
    sf::Text winnerText;
};

#endif

