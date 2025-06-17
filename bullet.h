#ifndef BULLET_H
#define BULLET_H

#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable {
public:
    Bullet(sf::Vector2f pos, sf::Vector2f direction);

    void markForDeletion() { toDelete = true; }
    void update(float dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isMarkedForDeletion() const { return toDelete; }
    bool isOffScreen() const;

    sf::FloatRect getBounds() const;

private:
    float speed = 300.f;

    bool toDelete = false;

    sf::CircleShape shape;
    sf::Vector2f velocity;

};


#endif // BULLET_H
