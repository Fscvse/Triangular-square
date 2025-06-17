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
    const std::vector<Bullet>& getBullets() const { return bullets; }

    sf::FloatRect getGlobalBounds() const override;
    sf::Vector2f getPosition() const override;
    void BulletCollisions(PlayerStats& player);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite sprite;                // ← używany sprite z teksturą
    std::vector<Bullet> bullets;
    sf::Clock fireClock;
    int attackDamage = 20;
    sf::Vector2f recoilOffset;
    float recoilDuration = 0.25f;  // czas odrzutu w sekundach
    sf::Clock recoilClock;
    bool isRecoiling = false;

};


#endif // ENEMYTRIANGLE_H
