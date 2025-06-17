#include "Player.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

PlayerStats::PlayerStats(float hp, float atkDmg, float atkSpeed)
    : maxHp(hp), currentHp(hp), attackDamage(atkDmg), attackSpeed(atkSpeed),
    isAlive(true), canAttack(true), barPosition(10, 10), barSize(300, 25),
    textureLoaded(false), bulletTextureLoaded(false), shootCooldown(0.2f) { // Initialize bulletTextureLoaded

    attackCooldown = 1.0f / attackSpeed;

    hpFullColor = sf::Color::Green;
    hpLowColor = sf::Color::Yellow;
    hpCriticalColor = sf::Color::Red;
    backgroundColor = sf::Color(50, 50, 50);
    borderColor = sf::Color::White;

    // You should load your font here, for example:
    // if (!font.loadFromFile("assets/arial.ttf")) {
    //     std::cout << "Error loading font!" << std::endl;
    // }

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

    // Ensure font is loaded before setting to text objects
    // if (font.loadFromFile("assets/arial.ttf")) { // Example font loading
    //     hpText.setFont(font);
    //     statsText.setFont(font);
    // } else {
    //     std::cout << "Error loading font for GUI!" << std::endl;
    // }
    hpText.setCharacterSize(16);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(barPosition.x + 5, barPosition.y + 3);

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
                << " | SPD: " << attackSpeed << "/s";
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

// Set HP bar position
void PlayerStats::setBarPosition(float x, float y) {
    barPosition = sf::Vector2f(x, y);

    hpBarBackground.setPosition(barPosition);
    hpBarForeground.setPosition(barPosition);
    hpBarBorder.setPosition(barPosition.x - 2, barPosition.y - 2);
    hpText.setPosition(barPosition.x + 5, barPosition.y + 3);
    statsText.setPosition(barPosition.x, barPosition.y + 40);
}

// Set HP bar size
void PlayerStats::setBarSize(float width, float height) {
    barSize = sf::Vector2f(width, height);

    hpBarBackground.setSize(barSize);
    hpBarBorder.setSize(sf::Vector2f(barSize.x + 4, barSize.y + 4));

    updateGUI();
}

// Set HP colors
void PlayerStats::setHpColors(sf::Color full, sf::Color low, sf::Color critical) {
    hpFullColor = full;
    hpLowColor = low;
    hpCriticalColor = critical;
    updateGUI();
}
// === PLAYER SPRITE FUNCTIONS ===

// Load player texture
bool PlayerStats::loadPlayerTexture(const std::string& texturePath) {
    if (playerTexture.loadFromFile(texturePath)) {
        playerSprite.setTexture(playerTexture);
        sf::FloatRect bounds = playerSprite.getLocalBounds();
        playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        playerSprite.setPosition(playerPosition);
        textureLoaded = true;
        std::cout << "Player texture loaded successfully: " << texturePath << std::endl;
        return true;
    } else {
        std::cout << "Error loading player texture: " << texturePath << std::endl;
        textureLoaded = false;
        return false;
    }
}

sf::FloatRect PlayerStats::getPlayerGlobalBounds() const {
    return playerSprite.getGlobalBounds();
}

// Set player position
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

// Get player position
sf::Vector2f PlayerStats::getPlayerPosition() const {
    return playerPosition;
}

// Move player
void PlayerStats::movePlayer(sf::Vector2f direction, float deltaTime, float speed) {
    playerPosition.x += direction.x * speed * deltaTime;
    playerPosition.y += direction.y * speed * deltaTime;
    if (textureLoaded) {
        playerSprite.setPosition(playerPosition);
    }
}

// Check if texture is loaded
bool PlayerStats::isTextureLoaded() const {
    return textureLoaded;
}

// Get sprite (for collisions etc.)
sf::Sprite& PlayerStats::getSprite() {
    return playerSprite;
}

const sf::Sprite& PlayerStats::getSprite() const {
    return playerSprite;
}

// Set sprite scale
void PlayerStats::setPlayerScale(float scaleX, float scaleY) {
    if (textureLoaded) {
        playerSprite.setScale(scaleX, scaleY);
    }
}

// === GETTERS ===
float PlayerStats::getMaxHp() const { return maxHp; }
float PlayerStats::getCurrentHp() const { return currentHp; }
float PlayerStats::getAttackDamage() const { return attackDamage; }
float PlayerStats::getAttackSpeed() const { return attackSpeed; }
bool PlayerStats::getIsAlive() const { return isAlive; }
bool PlayerStats::getCanAttack() const { return canAttack; }

float PlayerStats::getHpPercentage() const {
    return (currentHp / maxHp) * 100.0f;
}

// === SETTERS ===
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

// === MODIFIERS ===
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

// === ACTION FUNCTIONS ===
void PlayerStats::takeDamage(int damage) {
    if (!isAlive) return;
    float defense = 0; // Assuming player has no defense for now
    float finalDamage = std::max(1.0f, static_cast<float>(damage) - defense);
    modifyHp(-finalDamage);

    std::cout << "Player took " << finalDamage << " damage!" << std::endl;
}

float PlayerStats::performAttack() {
    updateAttackStatus();

    if (!canAttack || !isAlive) {
        return 0.0f;
    }

    attackClock.restart();
    canAttack = false;

    float finalDamage = attackDamage;

    std::cout << "Player attacks for " << finalDamage << " damage!" << std::endl;
    return finalDamage;
}



void PlayerStats::fullHeal() {
    setCurrentHp(maxHp);
}

void PlayerStats::resetToDefaults() {
    maxHp = 200.0f;
    currentHp = maxHp;
    attackDamage = 20.0f;
    attackSpeed = 1.0f;
    attackCooldown = 1.0f / attackSpeed;
    isAlive = true;
    canAttack = true;
    attackClock.restart();
    updateGUI();
}

// === DRAW FUNCTIONS ===
void PlayerStats::drawGUI(sf::RenderWindow& window) {
    updateGUI(); // Update before drawing

    // Draw HP bar elements
    window.draw(hpBarBackground);
    window.draw(hpBarForeground);
    window.draw(hpBarBorder);
    window.draw(hpText);
    window.draw(statsText);
}

// Draw with option to hide stats
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

// Draw player (sprite only)
void PlayerStats::drawPlayer(sf::RenderWindow& window) {
    if (textureLoaded) {
        window.draw(playerSprite);
    }
}

// Draw all - player and GUI
void PlayerStats::drawAll(sf::RenderWindow& window, bool showStats) {
    drawPlayer(window);
    drawGUI(window, showStats);
    drawBullets(window); // Ensure bullets are drawn as part of drawAll
}

// NEW: Load bullet texture for PlayerStats
bool PlayerStats::loadBulletTexture(const std::string& texturePath) {
    if (bulletTexture.loadFromFile(texturePath)) {
        bulletTextureLoaded = true;
        std::cout << "Bullet texture loaded successfully: " << texturePath << std::endl;
        return true;
    } else {
        std::cout << "Error loading bullet texture: " << texturePath << std::endl;
        bulletTextureLoaded = false;
        return false;
    }
}

// PlayerBullet constructor
PlayerBullet::PlayerBullet(sf::Vector2f startPos, sf::Vector2f direction, const sf::Texture& bulletTexture)
    : speed(300.f), markedForDeletion(false) {
    sprite.setTexture(bulletTexture); // Assign the pre-loaded texture
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(startPos);

    // Normalize direction and set velocity
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    velocity = direction * speed;
}

// Implement shootBullet
void PlayerStats::shootBullet(sf::Vector2f direction) {
    // Check cooldown
    if (shootClock.getElapsedTime().asSeconds() < shootCooldown) {
        return;
    }

    // Check if direction is not zero
    if (direction.x == 0.f && direction.y == 0.f) {
        return;
    }

    // Ensure bullet texture is loaded before creating bullets
    if (!bulletTextureLoaded) {
        std::cerr << "Cannot shoot: bullet texture not loaded!" << std::endl;
        return;
    }

    // Create a bullet from player's position
    sf::Vector2f bulletStartPos = playerSprite.getPosition(); // Use player sprite's center
    // Pass the pre-loaded bulletTexture to the PlayerBullet constructor
    bullets.push_back(PlayerBullet(bulletStartPos, direction, bulletTexture));

    // Reset cooldown
    shootClock.restart();
}

// Update bullets
void PlayerStats::updateBullets(float dt) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dt);

        // Remove bullets that are off-screen or marked for deletion
        sf::Vector2f pos = it->getBounds().getPosition();
        // Assuming a window size of 800x600 for the bounds check
        if (it->isMarkedForDeletion() ||
            pos.x < -50 || pos.x > 850 || // Added a small margin
            pos.y < -50 || pos.y > 650) { // Added a small margin
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

// Draw bullets
void PlayerStats::drawBullets(sf::RenderTarget& target) {
    for (const auto& bullet : bullets) {
        bullet.draw(target);
    }
}

// Getter for bullets
std::vector<PlayerBullet>& PlayerStats::getBullets() {
    return bullets;
}

// Implementations for PlayerBullet
void PlayerBullet::update(float dt) {
    sprite.move(velocity * dt);
}

void PlayerBullet::draw(sf::RenderTarget& target) const {
    target.draw(sprite);
}

sf::FloatRect PlayerBullet::getBounds() const {
    return sprite.getGlobalBounds();
}

bool PlayerBullet::isMarkedForDeletion() const {
    return markedForDeletion;
}

void PlayerBullet::markForDeletion() {
    markedForDeletion = true;
}
