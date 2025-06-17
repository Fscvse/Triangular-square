#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib> // dla system()

#include "functions.h"
#include "enemyCircle.h"
#include "enemyTriangle.h"
#include "enemySquare.h"
#include "Player.h"

using namespace std;

void gameLoop(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture)
{
    sf::Texture randTexture;
    std::vector<std::string> backgroundPaths = {
        "assets/game_background_1.png",
        "assets/game_background_2.png",
        "assets/game_background_3.png",
    };

    // Losowy wybór tła
    int randomIndex = rand() % backgroundPaths.size();
    if (!randTexture.loadFromFile(backgroundPaths[randomIndex])) {
        std::cerr << "Nie udalo się załadować tekstury tła: " << backgroundPaths[randomIndex] << std::endl;
    }
    randTexture.setRepeated(true);

    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setTexture(&randTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 600));


    sf::Texture circleTexture;
    if (!circleTexture.loadFromFile("assets/circle.png")) {
        std::cerr << "Nie udało się załadować tekstury circle.png" << std::endl;
    }
    sf::Texture squareTexture;
    if (!squareTexture.loadFromFile("assets/square.png")) {
        std::cerr << "Nie udało się załadować tekstury square.png" << std::endl;
    }
    sf::Texture triangleTexture;
    if (!triangleTexture.loadFromFile("assets/triangle.png")) {
        std::cerr << "Nie udało się załadować tekstury triangle.png" << std::endl;
    }

    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::Clock spawnClock;
    sf::Clock gameTimer; // Zegar do liczenia czasu gry


    // --- END NEW ---

    PlayerStats player;
    player.setBarPosition(50, 50);
    player.setBarSize(400, 30);
    player.setHpColors(sf::Color::Green, sf::Color::Yellow, sf::Color::Red);

    player.loadPlayerTexture("assets/player.png");
    player.setPlayerScale(0.4f, 0.4f);
    player.setPlayerPosition(400.f, 300.f);

    // Load the bullet texture once for the player
    if (!player.loadBulletTexture("assets/bullet.png")) {
        std::cerr << "Failed to load bullet texture! Bullets might not appear." << std::endl;
        // Handle error, maybe exit or use default shape
    }

    const float moveSpeed = 200.f;
    sf::Clock clock;
    bool gameRunning = true;
    bool videoPlayed = false;

    while (window.isOpen() && gameRunning)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                pauseMenu(window, font, gameRunning, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);
                clock.restart();
            }
        }

        float dt = clock.restart().asSeconds();

        // Odtwórz wideo po 5 minutach
        if (!videoPlayed && gameTimer.getElapsedTime().asSeconds() >= 300.0f) {
            videoPlayed = true;
            gameMusic.pause();

            std::string videoPath = "assets/video.mp4";
            VictoryScreen(window, font, gameRunning, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);

#ifdef _WIN32
            system(("start \"\" \"" + videoPath + "\"").c_str());
#elif __APPLE__
            system(("open \"" + videoPath + "\"").c_str());
#else
            system(("xdg-open \"" + videoPath + "\"").c_str());
#endif
            std::cout << "Odtwarzanie wideo po 5 minutach gry!" << std::endl;
        }


        // --- END NEW ---

        // Spawnowanie przeciwników co 2 sekundy
        if (spawnClock.getElapsedTime().asSeconds() > 2.0f) {
            int type = rand() % 3;
            sf::Vector2f spawnPos(static_cast<float>(rand() % 750), static_cast<float>(rand() % 550)); // Cast to float
            if (type == 0)
                enemies.push_back(std::make_unique<CircleEnemy>(spawnPos, circleTexture));
            else if (type == 1)
                enemies.push_back(std::make_unique<TriangleEnemy>(spawnPos, triangleTexture));
            else
                enemies.push_back(std::make_unique<SquareEnemy>(spawnPos, squareTexture));
            spawnClock.restart();
        }

        // Obsługa ruchu gracza
        sf::Vector2f direction(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y += 1.f;

        player.movePlayer(direction, dt * moveSpeed);

        // Strzelanie - strzałki kierunkowe
        sf::Vector2f shootDirection(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) shootDirection.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) shootDirection.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) shootDirection.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) shootDirection.y += 1.f;

        if (shootDirection.x != 0.f || shootDirection.y != 0.f) {
            player.shootBullet(shootDirection);
        }

        // Aktualizacja pocisków gracza
        player.updateBullets(dt);

        // Sprawdzanie kolizji pocisków gracza z przeciwnikami
        auto& bullets = player.getBullets();
        for (auto& bullet : bullets) {
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (bullet.getBounds().intersects((*it)->getGlobalBounds())) {
                    // Zadaj obrażenia w zależności od typu wroga
                    if (auto* triangle = dynamic_cast<TriangleEnemy*>((*it).get())) {
                        triangle->takeDamageTriangle(player.getAttackDamage());
                        if (!triangle->isAlive()) {
                            it = enemies.erase(it);
                        } else {
                            ++it;
                        }
                    } else if (auto* circle = dynamic_cast<CircleEnemy*>((*it).get())) {
                        circle->takeDamageCircle(player.getAttackDamage());
                        if (!circle->isAlive()) {
                            it = enemies.erase(it);
                        } else {
                            ++it;
                        }
                    } else if (auto* square = dynamic_cast<SquareEnemy*>((*it).get())) {
                        square->takeDamageSquare(player.getAttackDamage());
                        if (!square->isAlive()) {
                            it = enemies.erase(it);
                        } else {
                            ++it;
                        }
                    } else {
                        ++it; // If no specific enemy type, just move to the next
                    }

                    bullet.markForDeletion();
                    break; // Pocisk trafił, nie sprawdzaj dalej dla tego pocisku
                }
                else {
                    ++it;
                }
            }
        }

        // Aktualizacja przeciwników
        for (auto& enemy : enemies) {
            enemy->update(dt, player.getPlayerPosition());

            if (auto* triangle = dynamic_cast<TriangleEnemy*>(enemy.get())) {
                triangle->BulletCollisions(player);
            }
            if (auto* square = dynamic_cast<SquareEnemy*>(enemy.get())) {
                square->meleeAttackIfInRange(player);
            }
            if (auto* circle = dynamic_cast<CircleEnemy*>(enemy.get())) {
                circle->performAOEAttack(player);
            }
        }


        if (!player.getIsAlive())
        {
            gameOverScreen(window, font, gameRunning, backgroundTexture, buttonTexture);
            continue;
        }

        // Ograniczenie gracza do granic okna
        sf::Vector2f pos = player.getPlayerPosition();
        if (pos.x < 0) pos.x = 0;
        if (pos.y < 0) pos.y = 0;
        if (pos.x > 750) pos.x = 750;
        if (pos.y > 550) pos.y = 550;
        player.setPlayerPosition(pos.x, pos.y);

        // Rysowanie
        window.clear(sf::Color(200, 134, 23));

        // Rysowanie tła
        window.draw(background);

        for (auto& enemy : enemies)
            window.draw(*enemy);

        player.drawAll(window); // This already calls drawBullets(window) internally

        // Wyświetlanie czasu
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        int minutes = static_cast<int>(gameTimer.getElapsedTime().asSeconds()) / 60;
        int seconds = static_cast<int>(gameTimer.getElapsedTime().asSeconds()) % 60;
        timeText.setString("Czas: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
        timeText.setPosition(10, 10);
        window.draw(timeText);

        window.display();
    }
}
