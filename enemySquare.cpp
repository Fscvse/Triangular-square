#include "enemySquare.h"
#include <cmath>

SquareEnemy::SquareEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.28f, 0.28f); // zmniejszenie rozmiaru na ekranie
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(pos);
    health = 100;
}

void SquareEnemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;
    sprite.move(direction * 40.f * dt);  // slower speed
    // Obrót w stronę gracza
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle);
}

void SquareEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

