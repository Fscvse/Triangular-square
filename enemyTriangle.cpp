#include "enemyTriangle.h"
#include <cmath>
#include "Player.h"

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

    // Dodaj odrzut
    if (isRecoiling) {
        float t = recoilClock.getElapsedTime().asSeconds();
        if (t < recoilDuration) {
            // interpolacja wygaszania odrzutu
            float factor = 1.0f - (t / recoilDuration);
            sprite.move(recoilOffset * factor * dt * 10.f);  // *10 aby był efektowny
        } else {
            isRecoiling = false;
        }
    }

    // Strzelanie
    if (fireClock.getElapsedTime().asSeconds() >= 3.f) {
        bullets.emplace_back(sprite.getPosition(), direction);
        fireClock.restart();

        // Odrzut w przeciwnym kierunku
        recoilOffset = -direction * 10.f;  // cofnij o 10 pikseli
        isRecoiling = true;
        recoilClock.restart();
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

sf::FloatRect TriangleEnemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f TriangleEnemy::getPosition() const {
    return sprite.getPosition();
}

void TriangleEnemy::BulletCollisions(PlayerStats& player) {
    for (auto& bullet : bullets) {
        if (bullet.getBounds().intersects(player.getPlayerGlobalBounds())) {
            player.takeDamage(attackDamage);
            bullet.markForDeletion(); // musisz zaimplementować
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
    [](const Bullet& b) { return b.isMarkedForDeletion() || b.isOffScreen(); }), bullets.end());
}
void TriangleEnemy::takeDamageTriangle(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        // Opcjonalnie: ustaw flagę że wróg jest martwy
        // isDead = true;
    }

    // Opcjonalny efekt wizualny przy otrzymaniu obrażeń
    // np. zmiana koloru sprite'a na krótko
    sprite.setColor(sf::Color::Red);
    // Możesz dodać timer żeby przywrócić normalny kolor po czasie
}

// Opcjonalnie dodaj też funkcję sprawdzającą czy wróg żyje
bool TriangleEnemy::isAlive() const {
    return health > 0;
}

// I funkcję getter dla zdrowia
int TriangleEnemy::getHealth() const {
    return health;
}



