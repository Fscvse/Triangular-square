#include "enemyCircle.h"
#include <cmath>

CircleEnemy::CircleEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.33f, 0.33f); // zmniejszenie rozmiaru na ekranie
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(pos);
    health = 80;
}

void CircleEnemy::update(float dt, sf::Vector2f playerPos) {
    // Aktualizacja ataku
    attackTimer += dt;

    if (!isAttacking && attackTimer >= attackCooldown) {
        isAttacking = true;
        attackTimer = 0.f;
    }

    if (isAttacking) {
        sprite.rotate(rotationSpeed * dt);

        if (attackTimer >= attackDuration) {
            isAttacking = false;
            attackTimer = 0.f;
        }
    } else {
        // Ruch w stronę gracza tylko gdy nie atakuje
        sf::Vector2f direction = playerPos - sprite.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Minimalna odległość od gracza, w której wróg się zatrzymuje
        const float stopDistance = 50.f;

        if (length > stopDistance) {
            direction /= length; // Normalizacja
            sprite.move(direction * 60.f * dt);

            // Obrót w stronę gracza tylko gdy się porusza
            float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
            sprite.setRotation(angle);
        }
    }
}

void CircleEnemy::performAOEAttack(PlayerStats& player) {
    if (!isAttacking) return;

    float distance = std::hypot(player.getPlayerPosition().x - sprite.getPosition().x,
                                player.getPlayerPosition().y - sprite.getPosition().y);

    if (distance <= aoeRadius) {
        player.takeDamage(aoeDamage);
        std::cout << "CircleEnemy zadaje obrażenia obszarowe: " << aoeDamage << std::endl;
    }
}


void CircleEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

sf::FloatRect CircleEnemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f CircleEnemy::getPosition() const {
    return sprite.getPosition();
}

void CircleEnemy::takeDamageCircle(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
    }
}

bool CircleEnemy::isAlive() const {
    return health > 0;
}

int CircleEnemy::getHealth() const {
    return health;
}
