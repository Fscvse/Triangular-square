#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void gameOverScreen(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, sf::Texture& backgroundTexture, sf::Texture& buttonTexture) {
    // Tytuł "GAME OVER"
    sf::Text gameOverTitle;
    gameOverTitle.setFont(font);
    gameOverTitle.setString("game over");
    gameOverTitle.setCharacterSize(70);
    gameOverTitle.setFillColor(sf::Color::Red);
    gameOverTitle.setPosition(265.f, 80.f);

    // Tekst przycisku "QUIT"
    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("quit");
    quitText.setCharacterSize(75);
    quitText.setPosition(340.f, 325.f);
    quitText.setFillColor(sf::Color::White);

    // Prostokąt przycisku
    sf::RectangleShape quitButton(sf::Vector2f(400.f, 70.f));
    quitButton.setFillColor(sf::Color(128, 128, 128));
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
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (quitButton.getGlobalBounds().contains(mousePos)) {
            quitText.setFillColor(sf::Color::Red);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                //gameRunning = false;
                inGameOver = false;
            }
        } else {
            quitText.setFillColor(sf::Color::White);
        }

        // Rysowanie
        window.clear();
        window.draw(overlay);
        window.draw(gameOverTitle);
        window.draw(quitButton);
        window.draw(quitText);
        window.display();
    }
}
