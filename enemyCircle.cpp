#include "enemyCircle.h"
#include <cmath>

CircleEnemy::CircleEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.33f, 0.33f); // zmniejszenie rozmiaru na ekranie
    sprite.setPosition(pos);
}

void CircleEnemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;
    sprite.move(direction * 100.f * dt);
}

void CircleEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
