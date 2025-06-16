#ifndef ENEMYTRIANGLE_H
#define ENEMYTRIANGLE_H

#pragma once
#include "Enemy.h"
#include "Bullet.h"
#include <vector>

class TriangleEnemy : public Enemy {
public:
    TriangleEnemy(sf::Vector2f pos, sf::Texture& texture);
    void update(float dt, sf::Vector2f playerPos) override;
    const std::vector<Bullet>& getBullets() const { return bullets; }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite sprite;                // ← używany sprite z teksturą
    std::vector<Bullet> bullets;
    sf::Clock fireClock;
};


#endif // ENEMYTRIANGLE_H
