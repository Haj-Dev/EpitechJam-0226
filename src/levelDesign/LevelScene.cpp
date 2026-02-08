#include "LevelScene.hpp"
#include "../game.hpp"
#include "../menuScene/MenuScene.hpp"
#include <vector>

LevelScene::LevelScene(const std::string& levelPath)
    : _level(levelPath),
      _levelRenderer(_level),
      _playerAnimation(4, 0.8f),  // 4 frames, 0.8s per frame
      _playerPos(0, 0),
      _playerVelocity(0, 0),
      _playerOnGround(false) {
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

void LevelScene::update(float dt, game* gamePtr) {
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
        // Add dead body at current position (max 5 bodies)
        addDeadBody(_playerPos);
        // Respawn at start
        respawnPlayer();
        return; // Skip rest of update this frame
    }

    // Check for death (falling off screen)
    if (_playerPos.y >= DEATH_Y) {
        // Add dead body at death position (clamped to screen, max 5 bodies)
        dl::Vector2f deathPos = _playerPos;
        deathPos.y = DEATH_Y - 20.0f; // Place body slightly above death line
        if (deathPos.x < 0) deathPos.x = 0;
        if (deathPos.x > 400 - 16) deathPos.x = 400 - 16;
        addDeadBody(deathPos);

        // Respawn at start
        respawnPlayer();
        return; // Skip rest of update this frame
    }

    // Ceiling collision - check if jumping into blocks from below
    if (_playerVelocity.y < 0) {  // Only check when moving upward
        float blockBottomY = 0;
        if (checkBlockCollisionAbove(_playerPos, blockBottomY)) {
            // Hit ceiling - set player Y so their top aligns with block bottom
            _playerPos.y = blockBottomY;
            _playerVelocity.y = 0;  // Stop upward movement, start falling
        }
    }

    // Ground collision - check blocks, dead bodies
    float blockY = 0;
    float deadBodyY = 0;
    bool onBlock = checkBlockCollisionBelow(_playerPos, blockY);
    bool onDeadBody = checkDeadBodyCollision(_playerPos, deadBodyY);

    if (onBlock || onDeadBody) {
        // When both block and dead body, choose the highest one (smallest Y)
        if (onBlock && onDeadBody) {
            if (blockY < deadBodyY) {
                _playerPos.y = blockY - 16;
            } else {
                _playerPos.y = deadBodyY - 16;
            }
        } else if (onBlock) {
            // Set player Y so their bottom edge aligns with block top
            _playerPos.y = blockY - 16;  // Player sprite is 16 pixels tall
        } else {
            // Set player Y so their bottom edge aligns with dead body top
            _playerPos.y = deadBodyY - 16;  // Player on top of corpse
        }
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
            // Return to menu scene
            gamePtr->changeScene(std::make_unique<MenuScene>());
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

    // Draw the player with animation
    size_t playerFrame = _playerAnimation.getCurrentFrame();
    _playerSprite.loadFromSpriteSheet(_playerSheet, playerFrame);
    _playerSprite.setPosition(_playerPos);
    window.draw(_playerSprite);

    window.display();

    // Render UI on bottom screen
    window.clear(dl::BOTTOM_SCREEN, dl::Color(50, 50, 50));
    window.display();
}
