#ifndef PLAYER_H
#define PLAYER_H

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class PlayerStats {
private:
    float maxHp;
    float currentHp;
    float attackDamage;
    float attackSpeed;
    float defense;
    float criticalChance;
    float criticalMultiplier;

    sf::Clock attackClock;
    float attackCooldown;

    bool isAlive;
    bool canAttack;

    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    sf::Vector2f playerPosition;
    bool textureLoaded;

    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarForeground;
    sf::RectangleShape hpBarBorder;
    sf::Font font;
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

    // Statystyki
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
