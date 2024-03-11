
#include "game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Gol a Gol"), ballVelocity(0.f, 0.f), moveSpeed(300.0f), followBall(false), gameActive(true), showRetryButton(false) {
    energyBar.setSize(sf::Vector2f(200.0f, 30.0f));
    energyBar.setFillColor(sf::Color::Yellow);
    energyBar.setPosition(0, 550);

    topEnergyBar.setSize(sf::Vector2f(200.0f, 30.0f));
    topEnergyBar.setFillColor(sf::Color::Yellow);
    topEnergyBar.setPosition(0, 20);

    ball.setRadius(10.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(400 - ball.getRadius(), 300 - ball.getRadius());

    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    retryText.setFont(font);
    retryText.setString("Retry");
    retryText.setCharacterSize(24);
    retryText.setFillColor(sf::Color::White);
    retryText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    retryText.setOrigin(retryText.getLocalBounds().width / 2, retryText.getLocalBounds().height / 2);

    winnerText.setFont(font);
    winnerText.setCharacterSize(24);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 - 50));
    winnerText.setOrigin(winnerText.getLocalBounds().width / 2, winnerText.getLocalBounds().height / 2);

    returnText.setFont(font);
    returnText.setString("Return");
    returnText.setCharacterSize(24);
    returnText.setFillColor(sf::Color::White);
    returnText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 50));
    returnText.setOrigin(returnText.getLocalBounds().width / 2, returnText.getLocalBounds().height / 2);

}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        if (gameActive) {
            update(clock.restart());
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (!gameActive) {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                if (retryText.getGlobalBounds().contains(mousePos)) {
                    resetGame();
                } else if (returnText.getGlobalBounds().contains(mousePos)) {
                    window.close();  // Fecha a janela do jogo e retorna ao menu
                }
            }
            break;
        }
    }
}


void Game::update(sf::Time deltaTime) {
    if (!gameActive) return;

    float moveAmount = moveSpeed * deltaTime.asSeconds();

    // Movimentação da barra do jogador
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        energyBar.move(-moveAmount, 0);
        if (energyBar.getPosition().x < 0) {
            energyBar.setPosition(0, 550);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        energyBar.move(moveAmount, 0);
        if (energyBar.getPosition().x + energyBar.getSize().x > window.getSize().x) {
            energyBar.setPosition(window.getSize().x - energyBar.getSize().x, 550);
        }
    }

   // Movimentação da barra do CPU (topEnergyBar)
    if (followBall) {
        float targetX = ball.getPosition().x + ball.getRadius() - topEnergyBar.getSize().x / 2;
        float direction = targetX > topEnergyBar.getPosition().x ? 1.0f : -1.0f;
        float possibleMove = moveAmount * direction;

        // Calcula a nova posição com base na direção e na quantidade máxima de movimento
        float newXPosition = topEnergyBar.getPosition().x + possibleMove;

        // Evita que a topEnergyBar ultrapasse a bola
        if (direction > 0) {  // Movendo para a direita
            newXPosition = std::min(newXPosition, targetX);
        } else {  // Movendo para a esquerda
            newXPosition = std::max(newXPosition, targetX);
        }

        topEnergyBar.setPosition(std::clamp(newXPosition, 0.f, window.getSize().x - topEnergyBar.getSize().x), 20.f);
    }


    // Movimentação da bola
    if (ballStartTimer.getElapsedTime().asSeconds() >= 2.0f) {
        if (ballVelocity.x == 0.f && ballVelocity.y == 0.f) {
            ballVelocity.x = 200.f;
            ballVelocity.y = 200.f;
        }

        ball.move(ballVelocity.x * deltaTime.asSeconds(), ballVelocity.y * deltaTime.asSeconds());

        // Colisão com as bordas laterais
        if (ball.getPosition().x <= 0) {
            ballVelocity.x = -ballVelocity.x;
            ball.setPosition(0 + ball.getRadius(), ball.getPosition().y);
        } else if (ball.getPosition().x + ball.getRadius() * 2 >= window.getSize().x) {
            ballVelocity.x = -ballVelocity.x;
            ball.setPosition(window.getSize().x - ball.getRadius() * 2, ball.getPosition().y);
        }
    }

    // Verificação de colisões com as barras
    handleBarCollision(energyBar, true);
    handleBarCollision(topEnergyBar, false);

    // Verificação de ponto marcado
    if (ball.getPosition().y < 0) {
        gameActive = false;
        showRetryButton = true;
        winnerTextString = "Player Wins!";
    } else if (ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y) {
        gameActive = false;
        showRetryButton = true;
        winnerTextString = "CPU Wins!";
    }
}


void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(energyBar);
    window.draw(topEnergyBar);

    if (gameActive) {
        window.draw(ball);
    } else {
        // Quando o jogo não está ativo, mostra o texto de vitória, retry e return
        winnerText.setString(winnerTextString);
        winnerText.setOrigin(winnerText.getLocalBounds().width / 2, winnerText.getLocalBounds().height / 2);
        window.draw(winnerText);
        window.draw(retryText);
        window.draw(returnText); // Certifique-se de que returnText foi corretamente configurado
    }

    window.display();
}


void Game::resetGame() {
    ball.setPosition(400 - ball.getRadius(), 300 - ball.getRadius());
    ballVelocity = sf::Vector2f(0.f, 0.f);
    gameActive = true;
    showRetryButton = false;
    followBall = false;
    winnerTextString = "";
    ballStartTimer.restart();
}

void Game::handleBarCollision(const sf::RectangleShape& bar, bool isBottomBar) {
    if (ball.getGlobalBounds().intersects(bar.getGlobalBounds())) {
        const float accelerationFactor = 1.05f;
        const float maxSpeed = 200.f * 8;

        // Centro da bola
        float ballCenterX = ball.getPosition().x + ball.getRadius();
        float ballCenterY = ball.getPosition().y + ball.getRadius();

        // Lados e topo/inferior da barra
        float barLeft = bar.getPosition().x;
        float barRight = bar.getPosition().x + bar.getSize().x;
        float barTop = bar.getPosition().y;
        float barBottom = bar.getPosition().y + bar.getSize().y;

        // Verifica se a colisão é lateral ou no topo/inferior
        bool collisionSide = (ballCenterX < barLeft) || (ballCenterX > barRight);

        if (collisionSide) {
            // Colisão lateral
            ballVelocity.x = -ballVelocity.x * accelerationFactor;
            // Ajusta a posição da bola para evitar que ela fique presa dentro da barra
            if (ballCenterX < barLeft) {
                ball.setPosition(barLeft - ball.getRadius() * 2, ball.getPosition().y);
            } else {
                ball.setPosition(barRight + ball.getRadius(), ball.getPosition().y);
            }
        } else {
            // Colisão superior/inferior
            ballVelocity.y = -ballVelocity.y * accelerationFactor;
            // Ajusta a posição da bola para evitar que ela fique presa dentro da barra
            if (ballCenterY < barTop) {
                ball.setPosition(ball.getPosition().x, barTop - ball.getRadius() * 2);
            } else {
                ball.setPosition(ball.getPosition().x, barBottom + ball.getRadius());
            }
        }

        // Aplica aceleração e limita a velocidade
        ballVelocity.x = std::clamp(ballVelocity.x, -maxSpeed, maxSpeed);
        ballVelocity.y = std::clamp(ballVelocity.y, -maxSpeed, maxSpeed);

        // Ativa o movimento da barra superior após colisão com a barra inferior
        if (isBottomBar) {
            followBall = true;
        }
    }
}






