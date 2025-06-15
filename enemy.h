#ifndef ENEMY_H
#define ENEMY_H
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Enemy : public sf::Drawable {
public:
    virtual void update(float dt, sf::Vector2f playerPos) = 0;
    virtual ~Enemy() = default;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

#endif // ENEMY_H

