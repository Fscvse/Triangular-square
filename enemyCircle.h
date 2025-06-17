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
    sf::CircleShape shape;
    sf::Sprite sprite;
    bool isAttacking = false;
    float attackTimer = 0.f;
    float attackCooldown = 3.f; // co 3 sekundy próbuje zaatakować
    float attackDuration = 1.f; // atak trwa 1 sekundę
    float rotationSpeed = 360.f; // stopni na sekundę

    float aoeRadius = 100.f; // zasięg ataku obszarowego
    int aoeDamage = 1.f;
};

#endif // ENEMYCIRCLE_H
