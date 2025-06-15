#ifndef ENEMYTRIANGLE_H
#define ENEMYTRIANGLE_H
#pragma once
#include "Enemy.h"

class TriangleEnemy : public Enemy {
public:
    TriangleEnemy(sf::Vector2f pos, sf::Texture& texture);
    void update(float dt, sf::Vector2f playerPos) override;

protected:
    sf::Sprite sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::ConvexShape shape;
};

#endif // ENEMYTRIANGLE_H
