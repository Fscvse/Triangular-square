#include "enemySquare.h"
#include <cmath>

SquareEnemy::SquareEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.28f, 0.28f); // zmniejszenie rozmiaru na ekranie
    sprite.setPosition(pos);
}

void SquareEnemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;
    sprite.move(direction * 50.f * dt);  // slower speed
}

void SquareEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

