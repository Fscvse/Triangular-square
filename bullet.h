#ifndef BULLET_H
#define BULLET_H

#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable {
public:
    Bullet(sf::Vector2f pos, sf::Vector2f direction);
    void markForDeletion() { toDelete = true; }
    bool isMarkedForDeletion() const { return toDelete; }
    void update(float dt);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool toDelete = false;
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed = 300.f;
};


#endif // BULLET_H
