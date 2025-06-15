#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>  // dla std::unique_ptr

#include "functions.h"
#include "enemyCircle.h"
#include "enemyTriangle.h"
#include "enemySquare.h"


using namespace std;

void gameLoop(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic)
{
    sf::Texture circleTexture;
    if (!circleTexture.loadFromFile("assets/circle.png")) {
        std::cerr << "Nie udało się załadować tekstury triangle.png" << std::endl;
    }

    sf::Texture squareTexture;
    if (!squareTexture.loadFromFile("assets/square.png")) {
        std::cerr << "Nie udało się załadować tekstury triangle.png" << std::endl;
    }

    sf::Texture triangleTexture;
    if (!triangleTexture.loadFromFile("assets/triangle.png")) {
        std::cerr << "Nie udało się załadować tekstury triangle.png" << std::endl;
    }

    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::Clock spawnClock;

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

        // Spawnowanie przeciwników co 2 sekundy
        if (spawnClock.getElapsedTime().asSeconds() > 2.0f) {
            int type = rand() % 3;
            sf::Vector2f spawnPos(rand() % 750, rand() % 550);
            if (type == 0)
                enemies.push_back(std::make_unique<CircleEnemy>(spawnPos, circleTexture));
            else if (type == 1)
                enemies.push_back(std::make_unique<TriangleEnemy>(spawnPos, triangleTexture));

            else
                enemies.push_back(std::make_unique<SquareEnemy>(spawnPos, squareTexture));
            spawnClock.restart();
        }


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

        for (auto& enemy : enemies) {
            enemy->update(dt, player.getPosition());
        }


        // Ograniczenie gracza do granic okna
        sf::Vector2f pos = player.getPosition();
        if(pos.x < 0) player.setPosition(0, pos.y);
        if(pos.y < 0) player.setPosition(pos.x, 0);
        if(pos.x > 750) player.setPosition(750, pos.y);
        if(pos.y > 550) player.setPosition(pos.x, 550);

        window.clear(sf::Color(200, 134, 23));
        window.draw(player);
        for (auto& enemy : enemies) {
            window.draw(*enemy);
        }

        window.display();
    }
}
