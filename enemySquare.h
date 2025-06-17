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
    sf::Sprite sprite;
    float attackRange = 50.f;
    int attackDamage = 30;
    float attackCooldown = 1.0f;
    sf::Clock attackClock;
    bool isCharging = false;
    bool isAttacking = false;
    float chargeDuration = 0.15f;
    float attackDashDuration = 0.1f;
    sf::Clock attackPhaseClock;
    sf::Vector2f lastPlayerDirection;
    std::vector<BlurTrail> blurTrails;
    float trailLifetime = 0.15f; // ile sekund widoczny jest ślad
    float trailSpawnInterval = 0.02f;
    sf::Clock trailSpawnClock;

};


#endif // ENEMYSQUARE_H
