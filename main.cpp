#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "game2.h"

int main() {
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    while (true) {  // Loop que mantém o menu principal ativo
        sf::RenderWindow window(sf::VideoMode(800, 600), "Bem vindo ao Gol a Gol");

        sf::Text title("Gol a Gol", font, 50);
        title.setPosition(250, 50);
        title.setFillColor(sf::Color::White);

        sf::Text playVsCpu("1. Player vs CPU", font, 30);
        playVsCpu.setPosition(50, 200);
        playVsCpu.setFillColor(sf::Color::White);

        sf::Text playVsPlayer("2. Player vs Player", font, 30);
        playVsPlayer.setPosition(50, 300);
        playVsPlayer.setFillColor(sf::Color::White);

        sf::Text exitOption("3. Exit", font, 30);
        exitOption.setPosition(50, 400);
        exitOption.setFillColor(sf::Color::White);

        sf::RectangleShape underline(sf::Vector2f(0, 5));  // Linha de sublinhado com altura 5
        underline.setFillColor(sf::Color::White);

        while (window.isOpen()) {
            sf::Event event;
            auto mousePos = sf::Mouse::getPosition(window);  // Posição do mouse em relação à janela

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (playVsCpu.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();
                        Game game;
                        game.run();
                    } else if (playVsPlayer.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();
                        Game2 game2;
                        game2.run();
                    } else if (exitOption.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();
                        return 0;  // Encerra o programa
                    }
                }
            }

            window.clear();
            window.draw(title);
            window.draw(playVsCpu);
            window.draw(playVsPlayer);
            window.draw(exitOption);

            // Atualiza e desenha a linha de sublinhado baseando-se na posição do mouse
            underline.setSize(sf::Vector2f(playVsCpu.getGlobalBounds().width, 5));
            underline.setPosition(playVsCpu.getPosition().x, playVsCpu.getPosition().y + playVsCpu.getGlobalBounds().height + 5);
            if (playVsCpu.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                window.draw(underline);
            }
            underline.setSize(sf::Vector2f(playVsPlayer.getGlobalBounds().width, 5));
            underline.setPosition(playVsPlayer.getPosition().x, playVsPlayer.getPosition().y + playVsPlayer.getGlobalBounds().height + 5);
            if (playVsPlayer.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                window.draw(underline);
            }
            underline.setSize(sf::Vector2f(exitOption.getGlobalBounds().width, 5));
            underline.setPosition(exitOption.getPosition().x, exitOption.getPosition().y + exitOption.getGlobalBounds().height + 5);
            if (exitOption.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                window.draw(underline);
            }

            window.display();
        }
    }

    return 0;
}









