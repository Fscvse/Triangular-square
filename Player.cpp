#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

class PlayerStats {
private:
    // Podstawowe statystyki
    float maxHp;
    float currentHp;
    float attackDamage;
    float attackSpeed;
    float defense;
    float criticalChance;
    float criticalMultiplier;

    // System cooldownu ataku
    sf::Clock attackClock;
    float attackCooldown;

    // Statusy
    bool isAlive;
    bool canAttack;

    //Tekstury gracza
    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    sf::Vector2f playerPosition;
    bool textureLoaded;

    // Elementy GUI paska HP
    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarForeground;
    sf::RectangleShape hpBarBorder;
    sf::Font font;
    sf::Text hpText;
    sf::Text statsText;

    // Pozycja i rozmiar paska HP
    sf::Vector2f barPosition;
    sf::Vector2f barSize;

    // Kolory
    sf::Color hpFullColor;
    sf::Color hpLowColor;
    sf::Color hpCriticalColor;
    sf::Color backgroundColor;
    sf::Color borderColor;

public:
    // Konstruktor
    PlayerStats(float hp = 200.0f, float atkDmg = 10.0f, float atkSpeed = 1.0f)
        : maxHp(hp), currentHp(hp), attackDamage(atkDmg), attackSpeed(atkSpeed),
        isAlive(true), canAttack(true),
        barPosition(10, 10), barSize(300, 25) {

        attackCooldown = 1.0f / attackSpeed;

        // Ustawienie kolorów
        hpFullColor = sf::Color::Green;
        hpLowColor = sf::Color::Yellow;
        hpCriticalColor = sf::Color::Red;
        backgroundColor = sf::Color(50, 50, 50);
        borderColor = sf::Color::White;

        initializeGUI();
    }

    // Inicjalizacja elementów GUI
    void initializeGUI() {
        // Pasek HP - tło
        hpBarBackground.setPosition(barPosition);
        hpBarBackground.setSize(barSize);
        hpBarBackground.setFillColor(backgroundColor);

        // Pasek HP - wypełnienie
        hpBarForeground.setPosition(barPosition);
        hpBarForeground.setSize(barSize);
        hpBarForeground.setFillColor(hpFullColor);

        // Pasek HP - ramka
        hpBarBorder.setPosition(barPosition.x - 2, barPosition.y - 2);
        hpBarBorder.setSize(sf::Vector2f(barSize.x + 4, barSize.y + 4));
        hpBarBorder.setFillColor(sf::Color::Transparent);
        hpBarBorder.setOutlineThickness(2);
        hpBarBorder.setOutlineColor(borderColor);

        // Ładowanie czcionki (użyj domyślnej czcionki systemowej)
        // W prawdziwej aplikacji załaduj czcionkę z pliku
        // font.loadFromFile("arial.ttf");

        // Tekst HP
        hpText.setFont(font);
        hpText.setCharacterSize(16);
        hpText.setFillColor(sf::Color::White);
        hpText.setPosition(barPosition.x + 5, barPosition.y + 3);

        // Tekst statystyk
        statsText.setFont(font);
        statsText.setCharacterSize(14);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(barPosition.x, barPosition.y + 40);

        updateGUI();
    }

    // Aktualizacja GUI
    void updateGUI() {
        // Obliczanie procentu HP
        float hpPercentage = currentHp / maxHp;

        // Aktualizacja rozmiaru paska HP
        float newWidth = barSize.x * hpPercentage;
        hpBarForeground.setSize(sf::Vector2f(newWidth, barSize.y));

        // Zmiana koloru w zależności od HP
        if (hpPercentage > 0.6f) {
            hpBarForeground.setFillColor(hpFullColor);
        } else if (hpPercentage > 0.3f) {
            hpBarForeground.setFillColor(hpLowColor);
        } else {
            hpBarForeground.setFillColor(hpCriticalColor);
        }

        // Aktualizacja tekstu HP
        std::ostringstream hpStream;
        hpStream << std::fixed << std::setprecision(0) << currentHp << "/" << maxHp
                 << " (" << std::setprecision(1) << (hpPercentage * 100) << "%)";
        hpText.setString(hpStream.str());

        // Aktualizacja tekstu statystyk
        std::ostringstream statsStream;
        statsStream << "ATK: " << std::fixed << std::setprecision(1) << attackDamage
                    << " | SPD: " << attackSpeed << "/s"
                    << " | DEF: " << defense
                    << " | CRIT: " << std::setprecision(0) << (criticalChance * 100) << "%";
        statsText.setString(statsStream.str());

        // Aktualizacja statusu ataku
        updateAttackStatus();
    }

    // Ustawienie pozycji paska HP
    void setBarPosition(float x, float y) {
        barPosition = sf::Vector2f(x, y);

        hpBarBackground.setPosition(barPosition);
        hpBarForeground.setPosition(barPosition);
        hpBarBorder.setPosition(barPosition.x - 2, barPosition.y - 2);
        hpText.setPosition(barPosition.x + 5, barPosition.y + 3);
        statsText.setPosition(barPosition.x, barPosition.y + 40);
    }

    // Ustawienie rozmiaru paska HP
    void setBarSize(float width, float height) {
        barSize = sf::Vector2f(width, height);

        hpBarBackground.setSize(barSize);
        hpBarBorder.setSize(sf::Vector2f(barSize.x + 4, barSize.y + 4));

        updateGUI();
    }

    // Ustawienie kolorów
    void setHpColors(sf::Color full, sf::Color low, sf::Color critical) {
        hpFullColor = full;
        hpLowColor = low;
        hpCriticalColor = critical;
        updateGUI();
    }
    // === FUNKCJE SPRITE'A GRACZA ===

    // Ładowanie tekstury gracza
    bool loadPlayerTexture(const std::string& texturePath) {
        if (playerTexture.loadFromFile(texturePath)) {
            playerSprite.setTexture(playerTexture);
            playerSprite.setPosition(playerPosition);
            textureLoaded = true;
            std::cout << "Tekstura gracza załadowana pomyślnie: " << texturePath << std::endl;
            return true;
        } else {
            std::cout << "Błąd ładowania tekstury gracza: " << texturePath << std::endl;
            textureLoaded = false;
            return false;
        }
    }

    // Ustawienie pozycji gracza
    void setPlayerPosition(float x, float y) {
        playerPosition = sf::Vector2f(x, y);
        if (textureLoaded) {
            playerSprite.setPosition(playerPosition);
        }
    }

    void setPlayerPosition(sf::Vector2f position) {
        playerPosition = position;
        if (textureLoaded) {
            playerSprite.setPosition(playerPosition);
        }
    }

    // Pobieranie pozycji gracza
    sf::Vector2f getPlayerPosition() const {
        return playerPosition;
    }

    // Ruch gracza
    void movePlayer(sf::Vector2f direction, float deltaTime, float speed = 200.0f) {
        playerPosition.x += direction.x * speed * deltaTime;
        playerPosition.y += direction.y * speed * deltaTime;
        if (textureLoaded) {
            playerSprite.setPosition(playerPosition);
        }
    }

    // Sprawdzenie czy tekstura jest załadowana
    bool isTextureLoaded() const {
        return textureLoaded;
    }

    // Pobieranie sprite'a (do kolizji itp.)
    sf::Sprite& getSprite() {
        return playerSprite;
    }

    const sf::Sprite& getSprite() const {
        return playerSprite;
    }

    // Ustawienie skali sprite'a
    void setPlayerScale(float scaleX, float scaleY) {
        if (textureLoaded) {
            playerSprite.setScale(scaleX, scaleY);
        }
    }

    // === GETTERY ===
    float getMaxHp() const { return maxHp; }
    float getCurrentHp() const { return currentHp; }
    float getAttackDamage() const { return attackDamage; }
    float getAttackSpeed() const { return attackSpeed; }
    bool getIsAlive() const { return isAlive; }
    bool getCanAttack() const { return canAttack; }

    float getHpPercentage() const {
        return (currentHp / maxHp) * 100.0f;
    }

    // === SETTERY ===
    void setMaxHp(float hp) {
        maxHp = std::max(1.0f, hp);
        if (currentHp > maxHp) currentHp = maxHp;
        updateGUI();
    }

    void setCurrentHp(float hp) {
        currentHp = std::clamp(hp, 0.0f, maxHp);
        updateAliveStatus();
        updateGUI();
    }

    void setAttackDamage(float dmg) {
        attackDamage = std::max(0.0f, dmg);
        updateGUI();
    }

    void setAttackSpeed(float speed) {
        attackSpeed = std::max(0.1f, speed);
        attackCooldown = 1.0f / attackSpeed;
        updateGUI();
    }


    // === MODYFIKATORY ===
    void modifyHp(float amount) {
        setCurrentHp(currentHp + amount);
    }

    void modifyMaxHp(float amount) {
        setMaxHp(maxHp + amount);
    }

    void modifyAttackDamage(float amount) {
        setAttackDamage(attackDamage + amount);
    }

    void modifyAttackSpeed(float amount) {
        setAttackSpeed(attackSpeed + amount);
    }


    // === FUNKCJE AKCJI ===
    void takeDamage(float damage) {
        if (!isAlive) return;

        float finalDamage = std::max(1.0f, damage - defense);
        modifyHp(-finalDamage);

        std::cout << "Gracz otrzymał " << finalDamage << " obrażeń!" << std::endl;
    }

    void updateAttackStatus() {
        canAttack = (attackClock.getElapsedTime().asSeconds() >= attackCooldown);
    }

    float performAttack() {
        updateAttackStatus();

        if (!canAttack || !isAlive) {
            return 0.0f;
        }

        attackClock.restart();
        canAttack = false;

        float finalDamage = attackDamage;

        std::cout << "Gracz atakuje za " << finalDamage << " obrażeń!" << std::endl;
        return finalDamage;
    }

    void levelUp(int level = 1) {
        for (int i = 0; i < level; i++) {
            modifyMaxHp(20.0f);
            modifyAttackDamage(2.0f);
            modifyAttackSpeed(0.1f);
        }
        fullHeal();
        std::cout << "Gracz awansował o " << level << " poziom(ów)!" << std::endl;
    }

    void fullHeal() {
        setCurrentHp(maxHp);
    }

    void resetToDefaults() {
        maxHp = 200.0f;
        currentHp = maxHp;
        attackDamage = 10.0f;
        attackSpeed = 1.0f;
        attackCooldown = 1.0f / attackSpeed;
        isAlive = true;
        canAttack = true;
        attackClock.restart();
        updateGUI();
    }

    void updateAliveStatus() {
        isAlive = (currentHp > 0);
        if (!isAlive) {
            std::cout << "Gracz został pokonany!" << std::endl;
        }
    }

    // === FUNKCJE RYSOWANIA ===
    void drawGUI(sf::RenderWindow& window) {
        updateGUI();  // Aktualizacja przed rysowaniem

        // Rysowanie elementów paska HP
        window.draw(hpBarBackground);
        window.draw(hpBarForeground);
        window.draw(hpBarBorder);
        window.draw(hpText);
        window.draw(statsText);
    }

    // Rysowanie z możliwością ukrycia statystyk
    void drawGUI(sf::RenderWindow& window, bool showStats) {
        updateGUI();

        window.draw(hpBarBackground);
        window.draw(hpBarForeground);
        window.draw(hpBarBorder);
        window.draw(hpText);

        if (showStats) {
            window.draw(statsText);
        }
    }

    // Rysowanie tylko gracza (sprite)
    void drawPlayer(sf::RenderWindow& window) {
        if (textureLoaded) {
            window.draw(playerSprite);
        }
    }

    // Rysowanie wszystkiego - gracza i GUI
    void drawAll(sf::RenderWindow& window, bool showStats = true) {
        drawPlayer(window);
        drawGUI(window, showStats);
    }

};
