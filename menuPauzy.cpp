#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "functions.h"

using namespace std;

void pauseMenu(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture)
{
    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Powtarza teksturę na cały ekran

    // Tworzenie menu pauzy
    sf::Text pauseMenuText[3];
    string pauseOptions[] = {"resume", "settings", "quit"};
    int selectedIndex = 0;

    for(int i = 0; i < 3; ++i) {
        pauseMenuText[i].setFont(font);
        pauseMenuText[i].setString(pauseOptions[i]);
        pauseMenuText[i].setCharacterSize(75);
    }
    pauseMenuText[0].setPosition(310.f, 178.f);
    pauseMenuText[1].setPosition(300.f, 303.f);
    pauseMenuText[2].setPosition(345.f, 428.f);

    // Tworzenie przycisków pauzy
    std::vector<sf::RectangleShape> pauseButtons;
    auto makePauseButton = [&](float x, float y, float width, float height) {
        sf::RectangleShape butt(sf::Vector2f(width, height));
        butt.setTexture(&buttonTexture);
        butt.setPosition(x, y);
        return butt;
    };
    pauseButtons.push_back(makePauseButton(200.f, 200.f, 400.f, 70.f));
    pauseButtons.push_back(makePauseButton(200.f, 325.f, 400.f, 70.f));
    pauseButtons.push_back(makePauseButton(200.f, 450.f, 400.f, 70.f));

    // Tytuł "PAUSED"
    sf::Text pausedTitle;
    pausedTitle.setFont(font);
    pausedTitle.setString("PAUSED");
    pausedTitle.setCharacterSize(60);
    pausedTitle.setFillColor(sf::Color::Yellow);
    pausedTitle.setPosition(300.f, 50.f);

    // Semi-przezroczyste tło
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));

    bool inPause = true;
    while(inPause && window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) {
                window.close();
                gameRunning = false;
                return;
            }

            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Up) {
                    selectedIndex = (selectedIndex - 1 + 3) % 3;
                } else if(event.key.code == sf::Keyboard::Down) {
                    selectedIndex = (selectedIndex + 1) % 3;
                } else if(event.key.code == sf::Keyboard::Enter) {
                    if(selectedIndex == 0) {
                        // Resume
                        inPause = false;
                    }else if(selectedIndex==1) {
                        //settings
                        settingsMenu(window, font, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);
                    }else if(selectedIndex == 2) {
                        // Quit to menu
                        gameRunning = false;
                        inPause = false;
                    }
                } else if(event.key.code == sf::Keyboard::Escape) {
                    // Resume również na ESC
                    inPause = false;
                }
            }
        }

        // Obsługa myszy
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for(int i = 0; i < 3; ++i) {
            if(pauseButtons[i].getGlobalBounds().contains(mousePos)) {
                selectedIndex = i;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if(i == 0) {
                        // Resume
                        inPause = false;
                    } else if(selectedIndex==1) {
                        //settings
                        settingsMenu(window, font, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);
                    } else if(selectedIndex == 2) {
                        // Quit to menu
                        gameRunning = false;
                        inPause = false;
                    }
                }
            }
        }

        // Rysowanie menu pauzy
        window.clear();

        // Rysowanie tła
        window.draw(background);

        // Rysuj półprzezroczyste tło
        window.draw(overlay);

        // Rysuj tytuł
        window.draw(pausedTitle);

        // Rysuj przyciski
        for(auto& butt : pauseButtons) {
            window.draw(butt);
        }

        // Rysuj tekst menu
        for(int i = 0; i < 3; ++i) {
            if(i == selectedIndex)
                pauseMenuText[i].setFillColor(sf::Color::Red);
            else
                pauseMenuText[i].setFillColor(sf::Color::White);
            window.draw(pauseMenuText[i]);
        }

        window.display();
    }
}
void VictoryScreen(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture)
{
    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Powtarza teksturę na cały ekran

    // Tytuł "Wygrałeś"
    sf::Text victoryTitle;
    victoryTitle.setFont(font);
    victoryTitle.setString("Victory");
    victoryTitle.setCharacterSize(70);
    victoryTitle.setFillColor(sf::Color(127,255,212));
    victoryTitle.setPosition(285.f, 80.f);

    // Tekst przycisku "QUIT"
    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("quit");
    quitText.setCharacterSize(75);
    quitText.setPosition(340.f, 325.f);
    quitText.setFillColor(sf::Color(127,255,212));

    // Prostokąt przycisku
    sf::RectangleShape quitButton(sf::Vector2f(400.f, 70.f));
    quitButton.setTexture(&buttonTexture);
    quitButton.setPosition(200.f, 350.f);

    // Półprzezroczyste tło
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    bool inGameOver = true;
    while (inGameOver && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                gameRunning = false;
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape) {
                    gameRunning = false;
                    inGameOver = false;
                }
            }
        }

        // Obsługa myszy
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            gameRunning = false;
            inGameOver = false;
        }



        // Rysowanie
        window.clear();
        window.draw(background);
        window.draw(overlay);
        window.draw(victoryTitle);
        window.draw(quitButton);
        window.draw(quitText);
        window.display();
    }
}
