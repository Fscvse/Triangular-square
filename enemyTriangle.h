#ifndef ENEMYTRIANGLE_H
#define ENEMYTRIANGLE_H

#pragma once
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include <vector>

class TriangleEnemy : public Enemy {
public:
    TriangleEnemy(sf::Vector2f pos, sf::Texture& texture);

    void update(float dt, sf::Vector2f playerPos) override;
    void BulletCollisions(PlayerStats& player);
    void takeDamageTriangle(int damage);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isAlive() const;
    int getHealth() const;

    sf::FloatRect getGlobalBounds() const override;
    sf::Vector2f getPosition() const override;
    const std::vector<Bullet>& getBullets() const { return bullets; }

private:
    float recoilDuration = 0.25f;  // czas odrzutu w sekundach

    bool isRecoiling = false;
    int attackDamage = 20;

    sf::Sprite sprite;                // ← używany sprite z teksturą
    std::vector<Bullet> bullets;
    sf::Clock fireClock;
    sf::Vector2f recoilOffset;
    sf::Clock recoilClock;
};


#endif // ENEMYTRIANGLE_H
