#include "bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f direction) {
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(5.f, 5.f);
    shape.setPosition(pos);

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        velocity = direction / length * speed;
}

void Bullet::update(float dt) {
    shape.move(velocity * dt);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

bool Bullet::isOffScreen() const {
    sf::Vector2f pos = shape.getPosition();
    return (pos.x < 0 || pos.y < 0 || pos.x > 800 || pos.y > 600);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}
