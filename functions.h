#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

void pauseMenu(sf::RenderWindow& window, sf::Font& font, bool& gameRunning, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic);
void gameLoop(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic);
void settingsMenu(sf::RenderWindow& window, sf::Font& font, int& volMenu, int& volGame, sf::Music& menuMusic, sf::Music& gameMusic);


#endif // FUNCTIONS_H
