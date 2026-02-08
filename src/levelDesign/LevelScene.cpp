#include "LevelScene.hpp"
#include "../game.hpp"
#include "../menuScene/MenuScene.hpp"
#include "../levelSelectScene/LevelSelectScene.hpp"
#include <vector>

LevelScene::LevelScene(const std::string& levelPath)
    : _level(levelPath),
      _levelRenderer(_level),
      _levelPath(levelPath),
      _playerAnimation(4, 0.8f),  // 4 frames, 0.8s per frame
      _playerPos(0, 0),
      _playerVelocity(0, 0),
      _playerOnGround(false),
      _paused(false),
      _pauseMenuIndex(0),
      _pauseTextBuffer(256),
      _pauseTitleText(_pauseTextBuffer),
      _pauseResumeText(_pauseTextBuffer),
      _pauseLevelSelectText(_pauseTextBuffer),
      _pauseQuitText(_pauseTextBuffer),
      _pauseInfoText(_pauseTextBuffer) {
}

void LevelScene::onEnter() {
    // Initialize level rendering
    _levelRenderer.initialize();

    // Load player sprite
    _playerSheet.loadFromFile("romfs:/assets/jam_girl.t3x");
    _playerSprite.loadFromSpriteSheet(_playerSheet, 0);

    // Find spawn point from level data
    auto spawnTile = _level.getSpawnPoint();
    if (spawnTile.has_value()) {
        constexpr float tileSize = 16.0f;
        _spawnPoint = dl::Vector2f(
            static_cast<float>(spawnTile->x) * tileSize,
            static_cast<float>(spawnTile->y) * tileSize
        );
    } else {
        _spawnPoint = dl::Vector2f(16.0f, 100.0f); // Fallback
    }

    _playerPos = _spawnPoint;
    _playerVelocity = dl::Vector2f(0, 0);
    _playerOnGround = false;

    // Clear dead bodies from previous runs
    _deadBodies.clear();
    _deadBodySprites.clear();

    // Setup pause menu
    _paused = false;
    _pauseMenuIndex = 0;

    _pauseTitleText.setString("PAUSED");
    _pauseTitleText.setPosition(dl::Vector2f(110, 30));
    _pauseTitleText.setScale(dl::Vector2f(1.2f, 1.2f));
    _pauseTitleText.setColor(dl::Color(255, 255, 255));

    _pauseResumeText.setString("Resume");
    _pauseResumeText.setPosition(dl::Vector2f(100, 80));
    _pauseResumeText.setScale(dl::Vector2f(0.9f, 0.9f));

    _pauseLevelSelectText.setString("Level Select");
    _pauseLevelSelectText.setPosition(dl::Vector2f(100, 110));
    _pauseLevelSelectText.setScale(dl::Vector2f(0.9f, 0.9f));

    _pauseQuitText.setString("Main Menu");
    _pauseQuitText.setPosition(dl::Vector2f(100, 140));
    _pauseQuitText.setScale(dl::Vector2f(0.9f, 0.9f));

    // Setup pause platforms (level-specific)
    // Collision boxes sized to match text at 0.5 scale
    _pausePlatforms.clear();
    if (_levelPath.find("PausePlatform") != std::string::npos) {
        _pausePlatforms.push_back({dl::Vector2f(80.0f, 128.0f), 30.0f, 8.0f});
        _pausePlatforms.push_back({dl::Vector2f(160.0f, 96.0f), 55.0f, 8.0f});
        _pausePlatforms.push_back({dl::Vector2f(270.0f, 128.0f), 42.0f, 8.0f});

    }

    // Info text for platform pause mode
    _pauseInfoText.setString("START:Resume  B:Back");
    _pauseInfoText.setPosition(dl::Vector2f(30, 100));
    _pauseInfoText.setScale(dl::Vector2f(0.8f, 0.8f));
    _pauseInfoText.setColor(dl::Color(200, 200, 200));
}

void LevelScene::onExit() {
    // Clean up level resources here
}

void LevelScene::respawnPlayer() {
    _playerPos = _spawnPoint;
    _playerVelocity = dl::Vector2f(0, 0);
    _playerOnGround = false;
}

bool LevelScene::checkDeadBodyCollision(const dl::Vector2f& pos, float& deadBodyY) {
    // Check if player is standing on any dead body
    // When multiple bodies are stacked, find the highest one (smallest Y)
    bool foundCollision = false;
    float highestBodyY = 999999.0f;

    for (const auto& deadBody : _deadBodies) {
        // Simple AABB collision for standing on dead body
        if (pos.x + 16 > deadBody.x && pos.x < deadBody.x + 16 &&
            pos.y + 16 >= deadBody.y && pos.y + 16 <= deadBody.y + 16) {
            // Found a collision - check if this is the highest body
            if (deadBody.y < highestBodyY) {
                highestBodyY = deadBody.y;
                foundCollision = true;
            }
        }
    }

    if (foundCollision) {
        deadBodyY = highestBodyY;
        return true;
    }
    return false;
}

bool LevelScene::checkSawCollision(const dl::Vector2f& pos) {
    // Check if player is touching any saw using circle collision
    constexpr float tileSize = 16.0f;
    constexpr float sawRadius = 7.0f;  // Slightly smaller than 8 for forgiving collision

    for (std::size_t y = 0; y < _level.getHeight(); ++y) {
        for (std::size_t x = 0; x < _level.getWidth(); ++x) {
            if (_level.getTile(x, y) == Level::Tile::Saw) {
                // Saw center
                float sawCenterX = static_cast<float>(x) * tileSize + 8.0f;
                float sawCenterY = static_cast<float>(y) * tileSize + 8.0f;

                // Find closest point on player rectangle to saw center
                float closestX = sawCenterX;
                if (closestX < pos.x) closestX = pos.x;
                if (closestX > pos.x + 16) closestX = pos.x + 16;

                float closestY = sawCenterY;
                if (closestY < pos.y) closestY = pos.y;
                if (closestY > pos.y + 16) closestY = pos.y + 16;

                // Check distance from closest point to saw center
                float dx = sawCenterX - closestX;
                float dy = sawCenterY - closestY;
                if (dx * dx + dy * dy <= sawRadius * sawRadius) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool LevelScene::checkBlockCollisionBelow(const dl::Vector2f& pos, float& blockY) {
    // Check if there's a block directly below the player
    constexpr float tileSize = 16.0f;
    for (std::size_t y = 0; y < _level.getHeight(); ++y) {
        for (std::size_t x = 0; x < _level.getWidth(); ++x) {
            if (_level.getTile(x, y) == Level::Tile::Block) {
                float blockX = static_cast<float>(x) * tileSize;
                float blockTopY = static_cast<float>(y) * tileSize;

                // Check if player is above this block and overlapping horizontally
                if (pos.x + 16 > blockX && pos.x < blockX + 16 &&
                    pos.y + 16 >= blockTopY && pos.y + 16 <= blockTopY + 16) {
                    blockY = blockTopY;
                    return true;
                }
            }
        }
    }
    return false;
}

bool LevelScene::checkBlockCollisionAbove(const dl::Vector2f& pos, float& blockBottomY) {
    // Check if player is hitting a block from below (jumping into it)
    constexpr float tileSize = 16.0f;
    for (std::size_t y = 0; y < _level.getHeight(); ++y) {
        for (std::size_t x = 0; x < _level.getWidth(); ++x) {
            if (_level.getTile(x, y) == Level::Tile::Block) {
                float blockX = static_cast<float>(x) * tileSize;
                float blockY = static_cast<float>(y) * tileSize;
                float blockBottom = blockY + tileSize;

                // Check if player's top is hitting this block's bottom
                if (pos.x + 16 > blockX && pos.x < blockX + 16 &&
                    pos.y <= blockBottom && pos.y >= blockY) {
                    blockBottomY = blockBottom;
                    return true;
                }
            }
        }
    }
    return false;
}

bool LevelScene::checkBlockCollisionHorizontal(const dl::Vector2f& pos, float velocityX, float& correctedX) {
    // Check if there's a block to the left or right of the player
    constexpr float tileSize = 16.0f;
    for (std::size_t y = 0; y < _level.getHeight(); ++y) {
        for (std::size_t x = 0; x < _level.getWidth(); ++x) {
            if (_level.getTile(x, y) == Level::Tile::Block) {
                float blockX = static_cast<float>(x) * tileSize;
                float blockY = static_cast<float>(y) * tileSize;

                // Check if player overlaps vertically with this block
                if (pos.y + 16 > blockY && pos.y < blockY + 16) {
                    // Moving right - check right edge collision
                    if (velocityX > 0 && pos.x + 16 >= blockX && pos.x < blockX) {
                        correctedX = blockX - 16; // Stop at block's left edge
                        return true;
                    }
                    // Moving left - check left edge collision
                    if (velocityX < 0 && pos.x <= blockX + 16 && pos.x + 16 > blockX + 16) {
                        correctedX = blockX + 16; // Stop at block's right edge
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool LevelScene::checkDeadBodyCollisionHorizontal(const dl::Vector2f& pos, float velocityX, float& correctedX) {
    // Check if there's a dead body to the left or right of the player
    for (const auto& deadBody : _deadBodies) {
        // Check if player overlaps vertically with this dead body
        if (pos.y + 16 > deadBody.y && pos.y < deadBody.y + 16) {
            // Moving right - check right edge collision
            if (velocityX > 0 && pos.x + 16 >= deadBody.x && pos.x < deadBody.x) {
                correctedX = deadBody.x - 16; // Stop at dead body's left edge
                return true;
            }
            // Moving left - check left edge collision
            if (velocityX < 0 && pos.x <= deadBody.x + 16 && pos.x + 16 > deadBody.x + 16) {
                correctedX = deadBody.x + 16; // Stop at dead body's right edge
                return true;
            }
        }
    }
    return false;
}

void LevelScene::addDeadBody(const dl::Vector2f& pos) {
    // Enforce maximum dead body limit - cycle behavior
    if (_deadBodies.size() >= MAX_DEAD_BODIES) {
        // Clear all old bodies when limit reached, start fresh
        _deadBodies.clear();
        _deadBodySprites.clear();
    }

    // Add new dead body
    _deadBodies.push_back(pos);

    // Create sprite for dead body
    dl::Sprite deadSprite;
    deadSprite.loadFromSpriteSheet(_playerSheet, 0);
    _deadBodySprites.push_back(deadSprite);
}

bool LevelScene::checkPausePlatformBelow(const dl::Vector2f& pos, float& platformY) {
    // One-way platforms: skip when moving upward (can jump through from below)
    if (_playerVelocity.y < 0) return false;
    for (const auto& plat : _pausePlatforms) {
        if (pos.x + 16 > plat.position.x && pos.x < plat.position.x + plat.width &&
            pos.y + 16 >= plat.position.y && pos.y + 16 <= plat.position.y + plat.height) {
            platformY = plat.position.y;
            return true;
        }
    }
    return false;
}

void LevelScene::update(float dt, game* gamePtr) {
    // Toggle pause with START
    if (dl::Input::isKeyPressed(dl::Input::START)) {
        _paused = !_paused;
        _pauseMenuIndex = 0;
    }

    // Standard pause mode (no platforms): freeze game, navigate menu
    if (_paused && _pausePlatforms.empty()) {
        if (dl::Input::isKeyPressed(dl::Input::DUP) && _pauseMenuIndex > 0)
            _pauseMenuIndex--;
        if (dl::Input::isKeyPressed(dl::Input::DDOWN) && _pauseMenuIndex < PAUSE_MENU_COUNT - 1)
            _pauseMenuIndex++;

        _pauseResumeText.setColor(_pauseMenuIndex == 0 ? dl::Color(255, 255, 0) : dl::Color(200, 200, 200));
        _pauseLevelSelectText.setColor(_pauseMenuIndex == 1 ? dl::Color(255, 255, 0) : dl::Color(200, 200, 200));
        _pauseQuitText.setColor(_pauseMenuIndex == 2 ? dl::Color(255, 255, 0) : dl::Color(200, 200, 200));

        if (dl::Input::isKeyPressed(dl::Input::A)) {
            if (_pauseMenuIndex == 0) {
                _paused = false;
            } else if (_pauseMenuIndex == 1) {
                gamePtr->changeScene(std::make_unique<LevelSelectScene>());
            } else if (_pauseMenuIndex == 2) {
                gamePtr->changeScene(std::make_unique<MenuScene>());
            }
        }
        return;
    }

    // Platform pause mode: B goes back to level select
    if (_paused && !_pausePlatforms.empty()) {
        if (dl::Input::isKeyPressed(dl::Input::B)) {
            gamePtr->changeScene(std::make_unique<LevelSelectScene>());
            return;
        }
    }

    // --- Physics runs when unpaused OR in platform pause mode ---

    // Update level animations
    _levelRenderer.update(dt);

    // Handle input - left/right movement
    if (dl::Input::isKeyHeld(dl::Input::DLEFT)) {
        _playerVelocity.x = -MOVE_SPEED;
        _playerAnimation.update(dt);  // Animate only when moving
    } else if (dl::Input::isKeyHeld(dl::Input::DRIGHT)) {
        _playerVelocity.x = MOVE_SPEED;
        _playerAnimation.update(dt);  // Animate only when moving
    } else {
        _playerVelocity.x = 0;
        _playerAnimation.setFrame(0); // Idle: show first frame
    }

    // Handle jump - A button
    if (dl::Input::isKeyPressed(dl::Input::A) && _playerOnGround) {
        _playerVelocity.y = JUMP_SPEED;
        _playerOnGround = false;
    }

    // Apply gravity
    if (!_playerOnGround) {
        _playerVelocity.y += GRAVITY * dt;
    }

    // Update position
    _playerPos.x += _playerVelocity.x * dt;
    _playerPos.y += _playerVelocity.y * dt;

    // Check horizontal collisions with blocks and dead bodies
    float correctedX = _playerPos.x;
    if (checkBlockCollisionHorizontal(_playerPos, _playerVelocity.x, correctedX) ||
        checkDeadBodyCollisionHorizontal(_playerPos, _playerVelocity.x, correctedX)) {
        _playerPos.x = correctedX;
        _playerVelocity.x = 0; // Stop horizontal movement on collision
    }

    // Check for death from saw collision
    if (checkSawCollision(_playerPos)) {
        addDeadBody(_playerPos);
        respawnPlayer();
        return;
    }

    // Check for death (falling off screen)
    if (_playerPos.y >= DEATH_Y) {
        dl::Vector2f deathPos = _playerPos;
        deathPos.y = DEATH_Y - 20.0f;
        if (deathPos.x < 0) deathPos.x = 0;
        if (deathPos.x > 400 - 16) deathPos.x = 400 - 16;
        addDeadBody(deathPos);
        respawnPlayer();
        return;
    }

    // Ceiling collision - check if jumping into blocks from below
    if (_playerVelocity.y < 0) {
        float blockBottomY = 0;
        if (checkBlockCollisionAbove(_playerPos, blockBottomY)) {
            _playerPos.y = blockBottomY;
            _playerVelocity.y = 0;
        }
    }

    // Ground collision - check blocks, dead bodies, and pause platforms
    float blockY = 0;
    float deadBodyY = 0;
    float pausePlatY = 0;
    bool onBlock = checkBlockCollisionBelow(_playerPos, blockY);
    bool onDeadBody = checkDeadBodyCollision(_playerPos, deadBodyY);
    bool onPausePlat = _paused ? checkPausePlatformBelow(_playerPos, pausePlatY) : false;

    if (onBlock || onDeadBody || onPausePlat) {
        // Find highest surface (smallest Y)
        float highestY = 999999.0f;
        if (onBlock && blockY < highestY) highestY = blockY;
        if (onDeadBody && deadBodyY < highestY) highestY = deadBodyY;
        if (onPausePlat && pausePlatY < highestY) highestY = pausePlatY;

        _playerPos.y = highestY - 16;
        _playerVelocity.y = 0;
        _playerOnGround = true;
    } else {
        _playerOnGround = false;
    }

    // Keep player within screen horizontal bounds
    if (_playerPos.x < 0) _playerPos.x = 0;
    if (_playerPos.x > 400 - 16) _playerPos.x = 400 - 16;

    // Check if player reached the end flag (objective tile)
    auto objectiveTile = _level.getObjectivePoint();
    if (objectiveTile.has_value()) {
        constexpr float tileSize = 16.0f;
        float endFlagX = static_cast<float>(objectiveTile->x) * tileSize;
        if (_playerPos.x >= endFlagX - 8 && _playerPos.x <= endFlagX + 8 && _playerOnGround) {
            gamePtr->changeScene(std::make_unique<LevelSelectScene>());
        }
    }
}

void LevelScene::render(dl::RenderWindow& window) {
    // Render the level on top screen
    window.clear(dl::TOP_SCREEN, dl::Color(135, 206, 235)); // Sky blue background

    // Render all level tiles using LevelRenderer
    _levelRenderer.render(window);

    // Draw dead bodies as platforms
    for (std::size_t i = 0; i < _deadBodies.size(); ++i) {
        _deadBodySprites[i].setPosition(_deadBodies[i]);
        window.draw(_deadBodySprites[i]);
    }

    // Draw pause platform text on top screen when paused in platform mode
    if (_paused && !_pausePlatforms.empty()) {
        _pauseTitleText.setPosition(dl::Vector2f(170, 5));
        _pauseTitleText.setScale(dl::Vector2f(0.6f, 0.6f));
        _pauseTitleText.setColor(dl::Color(255, 255, 255));
        window.draw(_pauseTitleText);

        _pauseResumeText.setPosition(_pausePlatforms[0].position);
        _pauseResumeText.setScale(dl::Vector2f(0.5f, 0.5f));
        _pauseResumeText.setColor(dl::Color(255, 255, 255));
        window.draw(_pauseResumeText);

        _pauseLevelSelectText.setPosition(_pausePlatforms[1].position);
        _pauseLevelSelectText.setScale(dl::Vector2f(0.5f, 0.5f));
        _pauseLevelSelectText.setColor(dl::Color(255, 255, 255));
        window.draw(_pauseLevelSelectText);

        _pauseQuitText.setPosition(_pausePlatforms[2].position);
        _pauseQuitText.setScale(dl::Vector2f(0.5f, 0.5f));
        _pauseQuitText.setColor(dl::Color(255, 255, 255));
        window.draw(_pauseQuitText);
    }

    // Draw the player with animation
    size_t playerFrame = _playerAnimation.getCurrentFrame();
    _playerSprite.loadFromSpriteSheet(_playerSheet, playerFrame);
    _playerSprite.setPosition(_playerPos);
    window.draw(_playerSprite);

    window.display();

    // Render bottom screen
    if (_paused) {
        window.clear(dl::BOTTOM_SCREEN, dl::Color(20, 20, 40));
        if (!_pausePlatforms.empty()) {
            // Platform mode: show controls
            _pauseTitleText.setPosition(dl::Vector2f(110, 30));
            _pauseTitleText.setScale(dl::Vector2f(1.2f, 1.2f));
            _pauseTitleText.setColor(dl::Color(255, 255, 255));
            window.draw(_pauseTitleText);
            window.draw(_pauseInfoText);
        } else {
            // Standard mode: navigable menu
            _pauseTitleText.setPosition(dl::Vector2f(110, 30));
            _pauseTitleText.setScale(dl::Vector2f(1.2f, 1.2f));
            _pauseTitleText.setColor(dl::Color(255, 255, 255));
            window.draw(_pauseTitleText);

            _pauseResumeText.setPosition(dl::Vector2f(100, 80));
            _pauseResumeText.setScale(dl::Vector2f(0.9f, 0.9f));
            window.draw(_pauseResumeText);

            _pauseLevelSelectText.setPosition(dl::Vector2f(100, 110));
            _pauseLevelSelectText.setScale(dl::Vector2f(0.9f, 0.9f));
            window.draw(_pauseLevelSelectText);

            _pauseQuitText.setPosition(dl::Vector2f(100, 140));
            _pauseQuitText.setScale(dl::Vector2f(0.9f, 0.9f));
            window.draw(_pauseQuitText);
        }
        window.display();
    } else {
        window.clear(dl::BOTTOM_SCREEN, dl::Color(50, 50, 50));
        window.display();
    }
}
