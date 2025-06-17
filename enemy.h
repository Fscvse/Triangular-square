#ifndef ENEMY_H
#define ENEMY_H

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Enemy : public sf::Drawable {
public:
    Enemy() = default; // Domyślny konstruktor
     virtual void update(float dt, sf::Vector2f playerPos) = 0;
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }
    bool isDead() const {
        return health <= 0;
    }

    virtual sf::FloatRect getGlobalBounds() const = 0;
    virtual sf::Vector2f getPosition() const = 0;

    virtual ~Enemy() = default;
protected:
    int health = 1; // domyślna wartość, zostanie nadpisana przez klasy pochodne

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

#endif // ENEMY_H

