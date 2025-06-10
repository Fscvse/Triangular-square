#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "functions.h"

using namespace std;

void gameLoop(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic)
{
    sf::RectangleShape player(sf::Vector2f(50, 50));
    player.setPosition(100, 100);
    player.setFillColor(sf::Color(100, 100, 100));

    sf::Vector2f velocity(0.f, 0.f);
    const float moveSpeed = 200.f;
    sf::Clock clock;
    bool gameRunning = true;

    while(window.isOpen() && gameRunning)
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) {
                    // Otwórz menu pauzy
                    pauseMenu(window, font, gameRunning, volMenu, volGame, menuMusic, gameMusic);
                    clock.restart(); // Zresetuj zegar po pauzie
                }
            }
        }

        float dt = clock.restart().asSeconds();
        velocity.x = 0.f;
        velocity.y = 0.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            velocity.x = -moveSpeed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            velocity.x = moveSpeed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            velocity.y = -moveSpeed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            velocity.y = moveSpeed;

        player.move(velocity.x * dt, velocity.y * dt);

        // Ograniczenie gracza do granic okna
        sf::Vector2f pos = player.getPosition();
        if(pos.x < 0) player.setPosition(0, pos.y);
        if(pos.y < 0) player.setPosition(pos.x, 0);
        if(pos.x > 750) player.setPosition(750, pos.y);
        if(pos.y > 550) player.setPosition(pos.x, 550);

        window.clear(sf::Color(200, 134, 23));
        window.draw(player);
        window.display();
    }
}
