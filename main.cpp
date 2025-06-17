#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

#include "functions.h"

using namespace std;


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Geometrical Survivors");
    window.setFramerateLimit(60);

    // Ładowanie muzyki menu
    sf::Music menuMusic;
    int volMenu=50;
    if(!menuMusic.openFromFile("assets/sound.wav")) {
        std::cerr << "BŁĄD: Nie udalo sie zaladowac muzyki menu z sound.wav!" << std::endl;
        std::cerr << "Sprawdź czy plik jest w katalogu z .exe" << std::endl;
    } else {
        std::cout << "Muzyka menu załadowana pomyślnie!" << std::endl;
        menuMusic.setLoop(true);
        menuMusic.setVolume(volMenu);
        menuMusic.play();
    }

    // Ładowanie muzyki gry
    sf::Music gameMusic;
    int volGame=50;
    if(!gameMusic.openFromFile("assets/sound_game.wav")) {
        std::cerr << "BŁĄD: Nie udalo sie zaladowac muzyki gry z sound_game.wav!" << std::endl;
    } else {
        std::cout << "Muzyka gry załadowana pomyślnie!" << std::endl;
        gameMusic.setLoop(true);
        gameMusic.setVolume(volGame);
    }

    // Ładowanie czcionki
    sf::Font font;
    if(!font.loadFromFile("assets/Jacquard12-Regular.ttf")) {
        std::cerr << "Nie udalo sie zaladowac czcionki!" << std::endl;
        return 1;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/menu_background.png")) {
        std::cerr << "Nie udało się załadować tekstury tła!" << std::endl;
        return 1;
    }
    backgroundTexture.setRepeated(true);

    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("assets/button.png")) {
        std::cerr << "Nie udało się załadować tekstury przycisku!" << std::endl;
        return 1;
    }

    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Powtarza teksturę na cały ekran


    // Tworzenie napisów
    sf::Text menu[3];
    string options[] = {"play", "settings", "exit"};
    int selectedIndex = 0;

    for(int i = 0; i < 3; ++i) {
        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(75);
    }
    menu[0].setPosition(340.f, 178.f);
    menu[1].setPosition(300.f, 303.f);
    menu[2].setPosition(345.f, 428.f);

    // Tworzenie przycisków
    std::vector<sf::RectangleShape> buttons;
    auto makeButton = [&](float x, float y, float width, float height) {
        sf::RectangleShape butt(sf::Vector2f(width, height));
        butt.setTexture(&buttonTexture);
        butt.setPosition(x, y);
        return butt;
    };
    buttons.push_back(makeButton(200.f, 200.f, 400.f, 70.f));
    buttons.push_back(makeButton(200.f, 325.f, 400.f, 70.f));
    buttons.push_back(makeButton(200.f, 450.f, 400.f, 70.f));

    // Tytuł gry
    sf::Text pausedTitle;
    pausedTitle.setFont(font);
    pausedTitle.setString("Geometrical survivors");
    pausedTitle.setCharacterSize(60);
    pausedTitle.setFillColor(sf::Color::Yellow);
    pausedTitle.setPosition(150.f, 50.f);

    bool inGame = false;

    while(window.isOpen())
    {
        if(!inGame) {
            // === MENU ===
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed)
                    window.close();
                // Obsługa klawiatury
                if(event.type == sf::Event::KeyPressed) {
                    if(event.key.code == sf::Keyboard::Up) {
                        selectedIndex = (selectedIndex - 1 + 3) % 3;
                    } else if(event.key.code == sf::Keyboard::Down) {
                        selectedIndex = (selectedIndex + 1) % 3;
                    } else if(event.key.code == sf::Keyboard::Enter) {
                        if (selectedIndex == 0) {
                            // Play
                            cout << "Game starts!\n";
                            menuMusic.stop();
                            gameMusic.play();
                            inGame = true;
                        } else if(selectedIndex == 1){
                            // Settings
                            settingsMenu(window, font, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);
                        } else if(selectedIndex == 2){
                            // Exit
                            window.close();
                        }
                    }
                }
            }

            // Obsługa myszy
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            for(int i = 0; i < 3; ++i) {
                if(buttons[i].getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if(i == 0) {
                            // Play
                            cout << "Game starts!\n";
                            menuMusic.stop();
                            gameMusic.play();
                            inGame = true;
                        } else if(selectedIndex == 1){
                            // Settings
                            settingsMenu(window, font, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);
                        } else if(selectedIndex == 2){
                            // Exit
                            window.close();
                        }
                    }
                }
            }

            // Rysowanie menu
            window.clear();

            // Rysowanie tła
            window.draw(background);

            // Rysuj tytuł
            window.draw(pausedTitle);

            // Rysuj przyciski
            for(auto& butt : buttons) {
                window.draw(butt);
            }

            // Podświetlenie przycisków
            for(int i = 0; i < 3; ++i) {
                if(i == selectedIndex)
                    menu[i].setFillColor(sf::Color::Red);
                else
                    menu[i].setFillColor(sf::Color::White);
                window.draw(menu[i]);
            }

            window.display();

        } else {
            // === GRA ===
            gameLoop(window, font, volMenu, volGame, menuMusic, gameMusic, backgroundTexture, buttonTexture);

            // Po wyjściu z gry, wróć do menu
            inGame = false;
            gameMusic.stop();
            menuMusic.play();
        }
    }
    return 0;
}



