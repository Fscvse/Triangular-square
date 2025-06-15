#ifndef ENEMYSQUARE_H
#define ENEMYSQUARE_H

#pragma once
#include "Enemy.h"

class SquareEnemy : public Enemy {
public:
    SquareEnemy(sf::Vector2f pos, sf::Texture& texture);
    void update(float dt, sf::Vector2f playerPos) override;

protected:
    sf::Sprite sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape shape;
};

#endif // ENEMYSQUARE_H
