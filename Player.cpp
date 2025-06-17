#include "Player.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

PlayerStats::PlayerStats(float hp, float atkDmg, float atkSpeed)
    : maxHp(hp), currentHp(hp), attackDamage(atkDmg), attackSpeed(atkSpeed),
    isAlive(true), canAttack(true), barPosition(10, 10), barSize(300, 25) {

    attackCooldown = 1.0f / attackSpeed;

    hpFullColor = sf::Color::Green;
    hpLowColor = sf::Color::Yellow;
    hpCriticalColor = sf::Color::Red;
    backgroundColor = sf::Color(50, 50, 50);
    borderColor = sf::Color::White;

    initializeGUI();
}

void PlayerStats::initializeGUI() {
    hpBarBackground.setPosition(barPosition);
    hpBarBackground.setSize(barSize);
    hpBarBackground.setFillColor(backgroundColor);

    hpBarForeground.setPosition(barPosition);
    hpBarForeground.setSize(barSize);
    hpBarForeground.setFillColor(hpFullColor);

    hpBarBorder.setPosition(barPosition.x - 2, barPosition.y - 2);
    hpBarBorder.setSize(sf::Vector2f(barSize.x + 4, barSize.y + 4));
    hpBarBorder.setFillColor(sf::Color::Transparent);
    hpBarBorder.setOutlineThickness(2);
    hpBarBorder.setOutlineColor(borderColor);

    hpText.setFont(font);
    hpText.setCharacterSize(16);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(barPosition.x + 5, barPosition.y + 3);

    statsText.setFont(font);
    statsText.setCharacterSize(14);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(barPosition.x, barPosition.y + 40);

    updateGUI();
}

void PlayerStats::updateGUI() {
    float hpPercentage = currentHp / maxHp;
    float newWidth = barSize.x * hpPercentage;
    hpBarForeground.setSize(sf::Vector2f(newWidth, barSize.y));

    if (hpPercentage > 0.6f) hpBarForeground.setFillColor(hpFullColor);
    else if (hpPercentage > 0.3f) hpBarForeground.setFillColor(hpLowColor);
    else hpBarForeground.setFillColor(hpCriticalColor);

    std::ostringstream hpStream;
    hpStream << std::fixed << std::setprecision(0) << currentHp << "/" << maxHp
             << " (" << std::setprecision(1) << (hpPercentage * 100) << "%)";
    hpText.setString(hpStream.str());

    std::ostringstream statsStream;
    statsStream << "ATK: " << std::fixed << std::setprecision(1) << attackDamage
                << " | SPD: " << attackSpeed << "/s"
                << " | DEF: " << defense
                << " | CRIT: " << std::setprecision(0) << (criticalChance * 100) << "%";
    statsText.setString(statsStream.str());

    updateAttackStatus();
}

void PlayerStats::updateAttackStatus() {
    canAttack = (attackClock.getElapsedTime().asSeconds() >= attackCooldown);
}

void PlayerStats::updateAliveStatus() {
    isAlive = (currentHp > 0);
    if (!isAlive) std::cout << "Gracz został pokonany!" << std::endl;
}


// Ustawienie pozycji paska HP
void PlayerStats::setBarPosition(float x, float y) {
    barPosition = sf::Vector2f(x, y);

    hpBarBackground.setPosition(barPosition);
    hpBarForeground.setPosition(barPosition);
    hpBarBorder.setPosition(barPosition.x - 2, barPosition.y - 2);
    hpText.setPosition(barPosition.x + 5, barPosition.y + 3);
    statsText.setPosition(barPosition.x, barPosition.y + 40);
}

// Ustawienie rozmiaru paska HP
void PlayerStats::setBarSize(float width, float height) {
    barSize = sf::Vector2f(width, height);

    hpBarBackground.setSize(barSize);
    hpBarBorder.setSize(sf::Vector2f(barSize.x + 4, barSize.y + 4));

    updateGUI();
}

// Ustawienie kolorów
void PlayerStats::setHpColors(sf::Color full, sf::Color low, sf::Color critical) {
    hpFullColor = full;
    hpLowColor = low;
    hpCriticalColor = critical;
    updateGUI();
}
// === FUNKCJE SPRITE'A GRACZA ===

// Ładowanie tekstury gracza
bool PlayerStats::loadPlayerTexture(const std::string& texturePath) {
    if (playerTexture.loadFromFile(texturePath)) {
        playerSprite.setTexture(playerTexture);
        sf::FloatRect bounds = playerSprite.getLocalBounds();
        playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        playerSprite.setPosition(playerPosition);
        textureLoaded = true;
        std::cout << "Tekstura gracza załadowana pomyślnie: " << texturePath << std::endl;
        return true;
    } else {
        std::cout << "Błąd ładowania tekstury gracza: " << texturePath << std::endl;
        textureLoaded = false;
        return false;
    }
}

sf::FloatRect PlayerStats::getPlayerGlobalBounds() const {
    return playerSprite.getGlobalBounds();
}

// Ustawienie pozycji gracza
void PlayerStats::setPlayerPosition(float x, float y) {
    playerPosition = sf::Vector2f(x, y);
    if (textureLoaded) {
        playerSprite.setPosition(playerPosition);
    }
}

void PlayerStats::setPlayerPosition(sf::Vector2f position) {
    playerPosition = position;
    if (textureLoaded) {
        playerSprite.setPosition(playerPosition);
    }
}

// Pobieranie pozycji gracza
sf::Vector2f PlayerStats::getPlayerPosition() const {
    return playerPosition;
}

// Ruch gracza
void PlayerStats::movePlayer(sf::Vector2f direction, float deltaTime, float speed) {
    playerPosition.x += direction.x * speed * deltaTime;
    playerPosition.y += direction.y * speed * deltaTime;
    if (textureLoaded) {
        playerSprite.setPosition(playerPosition);
    }
}

// Sprawdzenie czy tekstura jest załadowana
bool PlayerStats::isTextureLoaded() const {
    return textureLoaded;
}

// Pobieranie sprite'a (do kolizji itp.)
sf::Sprite& PlayerStats::getSprite() {
    return playerSprite;
}

const sf::Sprite& PlayerStats::getSprite() const {
    return playerSprite;
}

// Ustawienie skali sprite'a
void PlayerStats::setPlayerScale(float scaleX, float scaleY) {
    if (textureLoaded) {
        playerSprite.setScale(scaleX, scaleY);
    }
}

// === GETTERY ===
float PlayerStats::getMaxHp() const { return maxHp; }
float PlayerStats::getCurrentHp() const { return currentHp; }
float PlayerStats::getAttackDamage() const { return attackDamage; }
float PlayerStats::getAttackSpeed() const { return attackSpeed; }
bool PlayerStats::getIsAlive() const { return isAlive; }
bool PlayerStats::getCanAttack() const { return canAttack; }

float PlayerStats::getHpPercentage() const {
    return (currentHp / maxHp) * 100.0f;
}

// === SETTERY ===
void PlayerStats::setMaxHp(float hp) {
    maxHp = std::max(1.0f, hp);
    if (currentHp > maxHp) currentHp = maxHp;
    updateGUI();
}

void PlayerStats::setCurrentHp(float hp) {
    currentHp = std::clamp(hp, 0.0f, maxHp);
    updateAliveStatus();
    updateGUI();
}

void PlayerStats::setAttackDamage(float dmg) {
    attackDamage = std::max(0.0f, dmg);
    updateGUI();
}

void PlayerStats::setAttackSpeed(float speed) {
    attackSpeed = std::max(0.1f, speed);
    attackCooldown = 1.0f / attackSpeed;
    updateGUI();
}


// === MODYFIKATORY ===
void PlayerStats::modifyHp(float amount) {
    setCurrentHp(currentHp + amount);
}

void PlayerStats::modifyMaxHp(float amount) {
    setMaxHp(maxHp + amount);
}

void PlayerStats::modifyAttackDamage(float amount) {
    setAttackDamage(attackDamage + amount);
}

void PlayerStats::modifyAttackSpeed(float amount) {
    setAttackSpeed(attackSpeed + amount);
}


// === FUNKCJE AKCJI ===
void PlayerStats::takeDamage(int damage) {
    if (!isAlive) return;

    float finalDamage = std::max(1.0f, damage - defense);
    modifyHp(-finalDamage);

    std::cout << "Gracz otrzymał " << finalDamage << " obrażeń!" << std::endl;
}

float PlayerStats::performAttack() {
    updateAttackStatus();

    if (!canAttack || !isAlive) {
        return 0.0f;
    }

    attackClock.restart();
    canAttack = false;

    float finalDamage = attackDamage;

    std::cout << "Gracz atakuje za " << finalDamage << " obrażeń!" << std::endl;
    return finalDamage;
}

void PlayerStats::levelUp(int level) {
    for (int i = 0; i < level; i++) {
        modifyMaxHp(20.0f);
        modifyAttackDamage(2.0f);
        modifyAttackSpeed(0.1f);
    }
    fullHeal();
    std::cout << "Gracz awansował o " << level << " poziom(ów)!" << std::endl;
}

void PlayerStats::fullHeal() {
    setCurrentHp(maxHp);
}

void PlayerStats::resetToDefaults() {
    maxHp = 200.0f;
    currentHp = maxHp;
    attackDamage = 10.0f;
    attackSpeed = 1.0f;
    attackCooldown = 1.0f / attackSpeed;
    isAlive = true;
    canAttack = true;
    attackClock.restart();
    updateGUI();
}

// === FUNKCJE RYSOWANIA ===
void PlayerStats::drawGUI(sf::RenderWindow& window) {
    updateGUI();  // Aktualizacja przed rysowaniem

    // Rysowanie elementów paska HP
    window.draw(hpBarBackground);
    window.draw(hpBarForeground);
    window.draw(hpBarBorder);
    window.draw(hpText);
    window.draw(statsText);
}

// Rysowanie z możliwością ukrycia statystyk
void PlayerStats::drawGUI(sf::RenderWindow& window, bool showStats) {
    updateGUI();

    window.draw(hpBarBackground);
    window.draw(hpBarForeground);
    window.draw(hpBarBorder);
    window.draw(hpText);

    if (showStats) {
        window.draw(statsText);
    }
}

// Rysowanie tylko gracza (sprite)
void PlayerStats::drawPlayer(sf::RenderWindow& window) {
    if (textureLoaded) {
        window.draw(playerSprite);
    }
}

// Rysowanie wszystkiego - gracza i GUI
void PlayerStats::drawAll(sf::RenderWindow& window, bool showStats) {
    drawPlayer(window);
    drawGUI(window, showStats);
}

