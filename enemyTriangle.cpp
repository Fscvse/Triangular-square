#include "enemyTriangle.h"
#include <cmath>

TriangleEnemy::TriangleEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture, true);
    sprite.setScale(0.33f, 0.33f); // zmniejszenie rozmiaru na ekranie
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(pos);
    health = 50;
}

void TriangleEnemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;
    }

    // Obracanie sprite'a w stronę gracza (domyślamy się, że górny wierzchołek to "góra" tekstury)
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    sprite.setRotation(angle + 90); // +90 zakłada że tekstura ma "górę" do góry

    if (length > 150.f) {
        sprite.move(direction * 50.f * dt); // ← poruszanie sprite’em
    }

    // Strzelanie
    if (fireClock.getElapsedTime().asSeconds() >= 3.f) {
        bullets.emplace_back(sprite.getPosition(), direction);
        fireClock.restart();
    }

    for (auto& bullet : bullets)
        bullet.update(dt);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
    [](const Bullet& b) { return b.isOffScreen(); }), bullets.end());
}

void TriangleEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states); // ← rysuj sprite
    for (const auto& bullet : bullets)
        target.draw(bullet, states);
}
