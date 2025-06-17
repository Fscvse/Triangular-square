#include "enemyCircle.h"
#include <cmath>

CircleEnemy::CircleEnemy(sf::Vector2f pos, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.33f, 0.33f); // zmniejszenie rozmiaru na ekranie
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(pos);
    health = 80;
    // W konstruktorze CircleEnemy
    if (!spinTexture.loadFromFile("assets/spin.png")) {
        std::cerr << "Nie udało się załadować spin.png" << std::endl;
    }
    spinSprite.setTexture(spinTexture);
    spinSprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    spinSprite.setScale(1.5f, 1.5f);  // dopasuj według potrzeby

    spinSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void CircleEnemy::update(float dt, sf::Vector2f playerPos) {
    // Aktualizacja ataku
    attackTimer += dt;

    if (!isAttacking && attackTimer >= attackCooldown) {
        isAttacking = true;
        attackTimer = 0.f;
    }

    if (isAttacking) {
        sprite.rotate(100 * dt);

        animationTimer += dt;

        if (animationTimer >= frameTime) {
            currentFrame = (currentFrame + 1) % frameCount;
            spinSprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }

        // KONIEC ATAKU po czasie attackDuration
        if (animationTimer >= attackDuration) {
            isAttacking = false;
            animationTimer = 0.f;
        }

        spinSprite.setPosition(sprite.getPosition());
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
    target.draw(sprite, states); // główny wróg

    if (isAttacking) {
        target.draw(spinSprite, states); // dodatkowa animacja
    }
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
