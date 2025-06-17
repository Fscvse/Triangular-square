#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "functions.h"

using namespace std;

void settingsMenu(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture)
{
    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Powtarza teksturę na cały ekran

    // Tworzenie menu ustawień
    sf::Text settingsMenuText[5];
    string settingsOptions[] = {"menu volume", "in game volume", "back", to_string(static_cast<int>(volMenu*0.1)), std::to_string(static_cast<int>(volGame*0.1))};
    int selectedIndex = 0;

    for(int i = 0; i < 5; ++i) {
        settingsMenuText[i].setFont(font);
        settingsMenuText[i].setString(settingsOptions[i]);
        settingsMenuText[i].setCharacterSize(60);
    }
    settingsMenuText[0].setPosition(100.f, 178.f);
    settingsMenuText[1].setPosition(100.f, 303.f);
    settingsMenuText[2].setPosition(345.f, 428.f);
    settingsMenuText[3].setPosition(700.f, 178.f);
    settingsMenuText[4].setPosition(700.f, 303.f);

    // Tworzenie przycisków
    std::vector<sf::ConvexShape> settingsButtons;
    auto makeSettingsButton = [&](float x1, float y1, float x2, float y2, float x3, float y3, float xpos, float ypos) {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(x1, y1));
        triangle.setPoint(1, sf::Vector2f(x2, y2));
        triangle.setPoint(2, sf::Vector2f(x3, y3));
        triangle.setFillColor(sf::Color(100, 100, 100));
        triangle.setOutlineColor(sf::Color::White);
        triangle.setOutlineThickness(3);
        triangle.setPosition(xpos, ypos);
        return triangle;
    };
    settingsButtons.push_back(makeSettingsButton(0.f, 0.f, 0.f, 20.f, 20.f, 10.f, 730.f, 220.f));
    settingsButtons.push_back(makeSettingsButton(0.f, 0.f, 0.f, 20.f, -20.f, 10.f, 690.f, 220.f));
    settingsButtons.push_back(makeSettingsButton(0.f, 0.f, 0.f, 20.f, 20.f, 10.f, 730.f, 345.f));
    settingsButtons.push_back(makeSettingsButton(0.f, 0.f, 0.f, 20.f, -20.f, 10.f, 690.f, 345.f));

    // Tytuł "SETTINGS"
    sf::Text settingsTitle;
    settingsTitle.setFont(font);
    settingsTitle.setString("settings");
    settingsTitle.setCharacterSize(75);
    settingsTitle.setFillColor(sf::Color::White);
    settingsTitle.setPosition(300.f, 50.f);

    bool inSettings = true;
    while(inSettings && window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedIndex = (selectedIndex - 1 + 3) % 3;
                } else if(event.key.code == sf::Keyboard::Down) {
                    selectedIndex = (selectedIndex + 1) % 3;
                } else if (event.key.code == sf::Keyboard::Right) {
                    if (selectedIndex == 0 && volMenu < 100) {
                        volMenu += 10;
                        menuMusic.setVolume(volMenu);
                    }
                    if (selectedIndex == 1 && volGame < 100) {
                        volGame += 10;
                        gameMusic.setVolume(volGame);
                    }
                } else if (event.key.code == sf::Keyboard::Left) {
                    if (selectedIndex == 0 && volMenu > 0) {
                        volMenu -= 10;
                        menuMusic.setVolume(volMenu);
                    }
                    if (selectedIndex == 1 && volGame > 0) {
                        volGame -= 10;
                        gameMusic.setVolume(volGame);
                    }
                } else if(event.key.code == sf::Keyboard::Enter) {
                    if(selectedIndex == 2) {
                        inSettings = false;
                    }
                } else if(event.key.code == sf::Keyboard::Escape) {
                    inSettings = false;
                }
            }
        }

        //Obsługa myszy
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < settingsButtons.size(); ++i) {
            if (settingsButtons[i].getGlobalBounds().contains(mousePos)) {
                settingsButtons[i].setOutlineColor(sf::Color::Red);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (i == 0 && volMenu < 100) {
                        volMenu += 10;
                        menuMusic.setVolume(volMenu);
                    }
                    if (i == 1 && volMenu > 0) {
                        volMenu -= 10;
                        menuMusic.setVolume(volMenu);
                    }
                    if (i == 2 && volGame < 100) {
                        volGame += 10;
                        gameMusic.setVolume(volGame);
                    }
                    if (i == 3 && volGame > 0) {
                        volGame -= 10;
                        gameMusic.setVolume(volGame);
                    }

                }
            } else {
                settingsButtons[i].setOutlineColor(sf::Color::White);
            }
        }

        // Ustawienie białego koloru przycisków
        for (int i = 0; i < 4; ++i) {
            settingsButtons[i].setOutlineColor(sf::Color::White);
        }

        // Podświetlenie przycisków na czerwono
        if (selectedIndex == 0) {
            settingsButtons[0].setOutlineColor(sf::Color::Red);
            settingsButtons[1].setOutlineColor(sf::Color::Red);
        } else if (selectedIndex == 1) {
            settingsButtons[2].setOutlineColor(sf::Color::Red);
            settingsButtons[3].setOutlineColor(sf::Color::Red);
        }

        settingsMenuText[3].setString(to_string(volMenu / 10));
        settingsMenuText[4].setString(to_string(volGame / 10));

        // Rysowanie menu pauzy
        window.clear();

        // Rysowanie tła
        window.draw(background);

        // Rysuj tytuł
        window.draw(settingsTitle);

        //Rysuj przyciski
        for (auto& triangle : settingsButtons) {
            window.draw(triangle);
        }

        // Rysuj tekst menu
        for(int i = 0; i < 3; ++i) {
            settingsMenuText[i].setFillColor(i == selectedIndex ? sf::Color::Red : sf::Color::White);
            window.draw(settingsMenuText[i]);
        }
        window.draw(settingsMenuText[3]);
        window.draw(settingsMenuText[4]);

        window.display();
    }
}

