#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

void pauseMenu(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture);
void gameLoop(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture);
void settingsMenu(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic, sf::Texture& backgroundTexture, sf::Texture& buttonTexture);
void gameOverScreen(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, sf::Texture& backgroundTexture, sf::Texture& buttonTexture);


#endif // FUNCTIONS_H
