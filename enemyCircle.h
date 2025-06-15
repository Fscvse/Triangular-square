#ifndef ENEMYCIRCLE_H
#define ENEMYCIRCLE_H
#pragma once
#include "Enemy.h"

class CircleEnemy : public Enemy {
public:
    CircleEnemy(sf::Vector2f pos, sf::Texture& texture);
    void update(float dt, sf::Vector2f playerPos) override;

protected:
    sf::Sprite sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::CircleShape shape;
};

#endif // ENEMYCIRCLE_H
