#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
using namespace std;

void gameLoop(sf::RenderWindow& window, sf::Music& gameMusic, sf::Font& font);
void pauseMenu(sf::RenderWindow& window, sf::Font& font, bool& gameRunning);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Geometrical Survivors");
    window.setFramerateLimit(60);

    // Ładowanie muzyki menu
    sf::Music menuMusic;
    if (!menuMusic.openFromFile("sound.wav")) {
        std::cerr << "BŁĄD: Nie udalo sie zaladowac muzyki menu z sound.wav!" << std::endl;
        std::cerr << "Sprawdź czy plik jest w katalogu z .exe" << std::endl;
    } else {
        std::cout << "Muzyka menu załadowana pomyślnie!" << std::endl;
        menuMusic.setLoop(true);
        menuMusic.setVolume(50);
        menuMusic.play();
    }

    // Ładowanie muzyki gry
    sf::Music gameMusic;
    if (!gameMusic.openFromFile("sound_game.wav")) {
        std::cerr << "BŁĄD: Nie udalo sie zaladowac muzyki gry z sound_game.wav!" << std::endl;
    } else {
        std::cout << "Muzyka gry załadowana pomyślnie!" << std::endl;
        gameMusic.setLoop(true);
        gameMusic.setVolume(50);
    }

    // Ładowanie czcionki
    sf::Font font;
    if (!font.loadFromFile("assets/Jacquard12-Regular.ttf")) {
        std::cerr << "Nie udalo sie zaladowac czcionki!" << std::endl;
        return 1;
    }

    // Tworzenie menu
    sf::Text menu[2];
    string options[] = {"play", "exit"};
    int selectedIndex = 0;

    for (int i = 0; i < 2; ++i) {
        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(75);
    }
    menu[0].setPosition(340.f, 178.f);
    menu[1].setPosition(345.f, 303.f);

    // Tworzenie przycisków
    std::vector<sf::RectangleShape> buttons;
    auto makeButton = [&](float x, float y, float width, float height) {
        sf::RectangleShape butt(sf::Vector2f(width, height));
        butt.setFillColor(sf::Color(128, 128, 128));
        butt.setPosition(x, y);
        return butt;
    };
    buttons.push_back(makeButton(200.f, 200.f, 400.f, 70.f));
    buttons.push_back(makeButton(200.f, 325.f, 400.f, 70.f));

    bool inGame = false;

    while (window.isOpen())
    {
        if (!inGame) {
            // === MENU ===
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedIndex = (selectedIndex - 1 + 2) % 2;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        selectedIndex = (selectedIndex + 1) % 2;
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedIndex == 0) {
                            cout << "Game starts!\n";
                            menuMusic.stop();
                            gameMusic.play();
                            inGame = true;
                        } else if (selectedIndex == 1) {
                            window.close();
                        }
                    }
                }
            }

            // Obsługa myszy
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            for (int i = 0; i < 2; ++i) {
                if (buttons[i].getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (i == 0) {
                            cout << "Game starts!\n";
                            menuMusic.stop();
                            gameMusic.play();
                            inGame = true;
                        } else if (i == 1) {
                            window.close();
                        }
                    }
                }
            }

            // Rysowanie menu
            window.clear();
            for (auto& butt : buttons) {
                window.draw(butt);
            }
            for (int i = 0; i < 2; ++i) {
                if (i == selectedIndex)
                    menu[i].setFillColor(sf::Color::Red);
                else
                    menu[i].setFillColor(sf::Color::White);
                window.draw(menu[i]);
            }
            window.display();
        } else {
            // === GRA ===
            gameLoop(window, gameMusic, font);

            // Po wyjściu z gry, wróć do menu
            inGame = false;
            gameMusic.stop();
            menuMusic.play();
        }
    }
    return 0;
}

void pauseMenu(sf::RenderWindow& window, sf::Font& font, bool& gameRunning)
{
    // Tworzenie menu pauzy
    sf::Text pauseMenuText[2];
    string pauseOptions[] = {"resume", "quit"};
    int selectedIndex = 0;

    for (int i = 0; i < 2; ++i) {
        pauseMenuText[i].setFont(font);
        pauseMenuText[i].setString(pauseOptions[i]);
        pauseMenuText[i].setCharacterSize(75);
    }
    pauseMenuText[0].setPosition(310.f, 178.f);
    pauseMenuText[1].setPosition(345.f, 303.f);

    // Tworzenie przycisków pauzy
    std::vector<sf::RectangleShape> pauseButtons;
    auto makePauseButton = [&](float x, float y, float width, float height) {
        sf::RectangleShape butt(sf::Vector2f(width, height));
        butt.setFillColor(sf::Color(128, 128, 128));
        butt.setPosition(x, y);
        return butt;
    };
    pauseButtons.push_back(makePauseButton(200.f, 200.f, 400.f, 70.f));
    pauseButtons.push_back(makePauseButton(200.f, 325.f, 400.f, 70.f));

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
    while (inPause && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                gameRunning = false;
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedIndex = (selectedIndex - 1 + 2) % 2;
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedIndex = (selectedIndex + 1) % 2;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedIndex == 0) {
                        // Resume
                        inPause = false;
                    } else if (selectedIndex == 1) {
                        // Quit to menu
                        gameRunning = false;
                        inPause = false;
                    }
                } else if (event.key.code == sf::Keyboard::Escape) {
                    // Resume również na ESC
                    inPause = false;
                }
            }
        }

        // Obsługa myszy
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < 2; ++i) {
            if (pauseButtons[i].getGlobalBounds().contains(mousePos)) {
                selectedIndex = i;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (i == 0) {
                        // Resume
                        inPause = false;
                    } else if (i == 1) {
                        // Quit to menu
                        gameRunning = false;
                        inPause = false;
                    }
                }
            }
        }

        // Rysowanie menu pauzy
        window.clear();

        // Rysuj półprzezroczyste tło
        window.draw(overlay);

        // Rysuj tytuł
        window.draw(pausedTitle);

        // Rysuj przyciski
        for (auto& butt : pauseButtons) {
            window.draw(butt);
        }

        // Rysuj tekst menu
        for (int i = 0; i < 2; ++i) {
            if (i == selectedIndex)
                pauseMenuText[i].setFillColor(sf::Color::Red);
            else
                pauseMenuText[i].setFillColor(sf::Color::White);
            window.draw(pauseMenuText[i]);
        }

        window.display();
    }
}

void gameLoop(sf::RenderWindow& window, sf::Music& gameMusic, sf::Font& font)
{
    sf::RectangleShape player(sf::Vector2f(50, 50));
    player.setPosition(100, 100);
    player.setFillColor(sf::Color(100, 100, 100));

    sf::Vector2f velocity(0.f, 0.f);
    const float moveSpeed = 200.f;
    sf::Clock clock;
    bool gameRunning = true;

    while (window.isOpen() && gameRunning)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    // Otwórz menu pauzy
                    pauseMenu(window, font, gameRunning);
                    clock.restart(); // Zresetuj zegar po pauzie
                }
            }
        }

        float dt = clock.restart().asSeconds();
        velocity.x = 0.f;
        velocity.y = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            velocity.x = -moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            velocity.x = moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            velocity.y = -moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            velocity.y = moveSpeed;

        player.move(velocity.x * dt, velocity.y * dt);

        // Ograniczenie gracza do granic okna
        sf::Vector2f pos = player.getPosition();
        if (pos.x < 0) player.setPosition(0, pos.y);
        if (pos.y < 0) player.setPosition(pos.x, 0);
        if (pos.x > 750) player.setPosition(750, pos.y);
        if (pos.y > 550) player.setPosition(pos.x, 550);

        window.clear(sf::Color(200, 134, 23));
        window.draw(player);
        window.display();
    }
}
