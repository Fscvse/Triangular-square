#ifndef ENEMYSQUARE_H
#define ENEMYSQUARE_H

#pragma once
#include "enemy.h"
#include "Player.h"

class SquareEnemy : public Enemy {
public:
    SquareEnemy(sf::Vector2f pos, sf::Texture& texture);

    void update(float dt, sf::Vector2f playerPos) override;
    void meleeAttackIfInRange(PlayerStats& playerStats);  // Nowa metoda
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void takeDamageSquare(int damage);

    bool isAlive() const;
    int getHealth() const;

    sf::FloatRect getGlobalBounds() const override;
    sf::Vector2f getPosition() const override;

    struct BlurTrail {
        sf::Sprite sprite;
        sf::Clock lifetimeClock;
    };

private:
    float attackRange = 50.f;
    float attackCooldown = 1.0f;
    float chargeDuration = 0.15f;
    float attackDashDuration = 0.1f;
    float trailLifetime = 0.15f; // ile sekund widoczny jest ślad
    float trailSpawnInterval = 0.02f;

    bool isCharging = false;
    bool isAttacking = false;
    int attackDamage = 30;

    sf::Sprite sprite;
    sf::Clock attackClock;
    sf::Clock attackPhaseClock;
    sf::Vector2f lastPlayerDirection;
    sf::Clock trailSpawnClock;
    std::vector<BlurTrail> blurTrails;
};


#endif // ENEMYSQUARE_H
