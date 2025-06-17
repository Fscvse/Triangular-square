#ifndef PLAYER_H
#define PLAYER_H

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector> // Add this include for std::vector

class PlayerBullet {
private:
    sf::Sprite sprite;
    // sf::Texture texture; // REMOVE THIS - texture will be managed by PlayerStats
    sf::Vector2f velocity;
    float speed;
    bool markedForDeletion;

public:
    // Modified constructor: takes a const reference to sf::Texture
    PlayerBullet(sf::Vector2f startPos, sf::Vector2f direction, const sf::Texture& bulletTexture);
    // Removed: bool loadTexture(const std::string& texturePath);
    void update(float dt);
    void draw(sf::RenderTarget& target) const;
    sf::FloatRect getBounds() const;
    bool isMarkedForDeletion() const;
    void markForDeletion();
};

class PlayerStats {
private:
    float maxHp;
    float currentHp;
    float attackDamage;
    float attackSpeed;
    std::vector<PlayerBullet> bullets;
    sf::Clock shootClock;
    float shootCooldown;

    sf::Clock attackClock;
    float attackCooldown;

    bool isAlive;
    bool canAttack;

    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    sf::Vector2f playerPosition;
    bool textureLoaded;

    sf::Texture bulletTexture; // NEW: Texture for bullets, loaded once
    bool bulletTextureLoaded; // NEW: To check if bullet texture is loaded

    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarForeground;
    sf::RectangleShape hpBarBorder;
    sf::Font font; // You'll need to load a font for this to work
    sf::Text hpText;
    sf::Text statsText;

    sf::Vector2f barPosition;
    sf::Vector2f barSize;

    sf::Color hpFullColor;
    sf::Color hpLowColor;
    sf::Color hpCriticalColor;
    sf::Color backgroundColor;
    sf::Color borderColor;

    void initializeGUI();
    void updateGUI();
    void updateAliveStatus();
    void updateAttackStatus();

public:
    PlayerStats(float hp = 200.0f, float atkDmg = 10.0f, float atkSpeed = 1.0f);
    // NEW: Method to load the bullet texture for PlayerStats
    bool loadBulletTexture(const std::string& texturePath);
    void shootBullet(sf::Vector2f direction);
    void updateBullets(float dt);
    void drawBullets(sf::RenderTarget& target);
    std::vector<PlayerBullet>& getBullets();

    // GUI
    void setBarPosition(float x, float y);
    void setBarSize(float width, float height);
    void setHpColors(sf::Color full, sf::Color low, sf::Color critical);

    // Sprite
    bool loadPlayerTexture(const std::string& texturePath);
    void setPlayerPosition(float x, float y);
    void setPlayerPosition(sf::Vector2f position);
    sf::Vector2f getPlayerPosition() const;
    void movePlayer(sf::Vector2f direction, float deltaTime, float speed = 2.0f);
    bool isTextureLoaded() const;
    void setPlayerScale(float scaleX, float scaleY);
    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;
    sf::FloatRect getPlayerGlobalBounds() const;

    // Stats
    float getMaxHp() const;
    float getCurrentHp() const;
    float getAttackDamage() const;
    float getAttackSpeed() const;
    bool getIsAlive() const;
    bool getCanAttack() const;
    float getHpPercentage() const;

    void setMaxHp(float hp);
    void setCurrentHp(float hp);
    void setAttackDamage(float dmg);
    void setAttackSpeed(float speed);

    void modifyHp(float amount);
    void modifyMaxHp(float amount);
    void modifyAttackDamage(float amount);
    void modifyAttackSpeed(float amount);

    void takeDamage(int damage);
    float performAttack();
    void levelUp(int level = 1);
    void fullHeal();
    void resetToDefaults();

    void drawGUI(sf::RenderWindow& window);
    void drawGUI(sf::RenderWindow& window, bool showStats);
    void drawPlayer(sf::RenderWindow& window);
    void drawAll(sf::RenderWindow& window, bool showStats = true);
};

#endif // PLAYER_H
