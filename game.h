
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void resetGame();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void handleBarCollision(const sf::RectangleShape& bar, bool isBottomBar);
    sf::Text returnText;
    sf::RenderWindow window;
    sf::RectangleShape energyBar;
    sf::RectangleShape topEnergyBar;
    sf::CircleShape ball;
    sf::Vector2f ballVelocity;
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
