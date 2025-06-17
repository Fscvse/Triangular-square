#include "enemySquare.h"
#include <cmath>

SquareEnemy::SquareEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.28f, 0.28f);
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

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);

    if (isAttacking) {
        float elapsed = attackPhaseClock.getElapsedTime().asSeconds();

        if (isCharging) {
            // Cofanie się lekko
            if (elapsed < chargeDuration) {
                sprite.move(-lastPlayerDirection * 40.f * dt); // delikatne cofnięcie
            } else {
                isCharging = false;
                attackPhaseClock.restart();
            }
        } else {
            // Uderzenie do przodu
            if (elapsed < attackDashDuration) {
                sprite.move(lastPlayerDirection * 500.f * dt);// szybki dash

                if (trailSpawnClock.getElapsedTime().asSeconds() >= trailSpawnInterval) {
                    BlurTrail trail;
                    trail.sprite = sprite;
                    trail.sprite.setColor(sf::Color(255, 255, 255, 100)); // półprzezroczysty
                    blurTrails.push_back(trail);
                    trailSpawnClock.restart();
                }
            } else {
                isAttacking = false;
            }
        }
    } else {
        // Normalne poruszanie się
        const float stopDistance = 35.f;
        if (length > stopDistance) {
            sprite.move(direction * 40.f * dt);
        }
    }
    blurTrails.erase(std::remove_if(blurTrails.begin(), blurTrails.end(),
    [this](const BlurTrail& t) {
        return t.lifetimeClock.getElapsedTime().asSeconds() > trailLifetime;
    }), blurTrails.end());
}

void SquareEnemy::meleeAttackIfInRange(PlayerStats& playerStats) {
    float distance = std::hypot(
        playerStats.getPlayerPosition().x - sprite.getPosition().x,
        playerStats.getPlayerPosition().y - sprite.getPosition().y
        );

    if (!isAttacking && distance <= attackRange && attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
        playerStats.takeDamage(attackDamage);
        attackClock.restart();

        // Rozpocznij animację ataku
        isAttacking = true;
        isCharging = true;
        attackPhaseClock.restart();

        sf::Vector2f dir = playerStats.getPlayerPosition() - sprite.getPosition();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0)
            dir /= len;
        lastPlayerDirection = dir;
    }
}

void SquareEnemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& trail : blurTrails) {
        target.draw(trail.sprite, states);
    }

    target.draw(sprite, states);
}

sf::FloatRect SquareEnemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f SquareEnemy::getPosition() const {
    return sprite.getPosition();
}
