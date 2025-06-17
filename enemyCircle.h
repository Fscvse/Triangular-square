#ifndef ENEMYCIRCLE_H
#define ENEMYCIRCLE_H
#pragma once
#include "Enemy.h"
#include "Player.h"

class CircleEnemy : public Enemy {
public:
    CircleEnemy(sf::Vector2f pos, sf::Texture& texture);

    void update(float dt, sf::Vector2f playerPos) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void performAOEAttack(PlayerStats& player);
    void takeDamageCircle(int damage);

    bool isAlive() const;
    int getHealth() const;

    sf::FloatRect getGlobalBounds() const override;
    sf::Vector2f getPosition() const override;

private:
    float attackTimer = 0.f;
    float attackCooldown = 3.f; // co 3 sekundy próbuje zaatakować
    float attackDuration = 1.f; // atak trwa 1 sekundę
    float rotationSpeed = 360.f; // stopni na sekundę
    float aoeRadius = 100.f; // zasięg ataku obszarowego
    float frameTime = 0.05f;          // czas jednej klatki
    float animationTimer = 0.f;

    bool isAttacking = false;
    int aoeDamage = 1.f;
    int frameCount = 10;              // liczba klatek (np. 10)
    int currentFrame = 0;
    int frameWidth = 64;              // dostosuj do rozmiaru klatki
    int frameHeight = 64;

    sf::CircleShape shape;
    sf::Sprite sprite;
    sf::Texture spinTexture;
    sf::Sprite spinSprite;



};

#endif // ENEMYCIRCLE_H
