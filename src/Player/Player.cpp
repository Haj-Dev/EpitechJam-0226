/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** Player
*/

#include "Player.hpp"

#include "../levelDesign/levelDesign.hpp"

#include <cmath>

Player::Player() {
    _spriteSheet.loadFromFile("romfs:/assets/jam_girl/jam_girl.t3x");
    _sprite       = std::make_unique<dl::Sprite>();
    _currentFrame = AnimFrame::Walk1;
    setAnimationFrame(AnimFrame::Idle);
}

Player::~Player() {}

void Player::setAnimationFrame(Player::AnimFrame frame) {
    if (!_sprite) {
        return;
    }
    if (frame == _currentFrame) {
        return;
    }
    _currentFrame = frame;
    _sprite->loadFromSpriteSheet(_spriteSheet, static_cast<std::size_t>(frame));
    _sprite->setPosition(_position);
}

const dl::Vector2f& Player::getPosition() const {
    return _position;
}

void Player::setPosition(const dl::Vector2f& position) {
    _position = position;
    if (_sprite) {
        _sprite->setPosition(_position);
    }
}

void Player::move(const dl::Vector2f& offset) {
    _position.x += offset.x;
    _position.y += offset.y;
    if (_sprite) {
        _sprite->setPosition(_position);
    }
}

const dl::Vector2f& Player::getVelocity() const {
    return _velocity;
}

void Player::setVelocity(const dl::Vector2f& velocity) {
    _velocity = velocity;
}

const dl::Vector2f& Player::getAcceleration() const {
    return _acceleration;
}

void Player::setAcceleration(const dl::Vector2f& acceleration) {
    _acceleration = acceleration;
}

void Player::setGroundY(float groundY) {
    _groundY = groundY;
}

void Player::handleInput(bool moveLeft, bool moveRight, bool jumpPressed) {
    _moveDir = 0;
    if (moveLeft) {
        _moveDir -= 1;
    }
    if (moveRight) {
        _moveDir += 1;
    }
    if (jumpPressed) {
        _jumpQueued = true;
    }
}

void Player::update(float dtSeconds, const Level& level) {
    applyInput(dtSeconds);

    const float  tileSize = Level::kTileSize;
    const float  epsilon  = 0.001f;
    dl::Vector2f nextPos  = _position;

    resolveHorizontal(level, nextPos, dtSeconds, tileSize, epsilon);
    resolveVertical(level, nextPos, dtSeconds, tileSize, epsilon);
    probeGround(level, nextPos, tileSize, epsilon);

    _position = nextPos;
    handleHazards(level, tileSize, epsilon);
    updateAnimation(dtSeconds);
    syncSprite();
}

void Player::applyInput(float dtSeconds) {
    _velocity.x = static_cast<float>(_moveDir) * kMoveSpeed;
    if (_onGround && _jumpQueued) {
        _velocity.y = -kJumpSpeed;
    }
    _jumpQueued = false;

    _velocity.x += _acceleration.x * dtSeconds;
    _velocity.y += (kGravity + _acceleration.y) * dtSeconds;
}

bool Player::isSolidTile(const Level& level, int tileX, int tileY) {
    if (tileX < 0 || tileY < 0) {
        return false;
    }
    return level.getTile(static_cast<std::size_t>(tileX), static_cast<std::size_t>(tileY)) == Level::Tile::Block;
}

void Player::resolveHorizontal(const Level& level, dl::Vector2f& nextPos, float dtSeconds, float tileSize, float epsilon) {
    const float deltaX = _velocity.x * dtSeconds;
    if (deltaX == 0.0f) {
        return;
    }

    nextPos.x += deltaX;
    const float left   = nextPos.x;
    const float right  = nextPos.x + kHitboxWidth - epsilon;
    const float top    = nextPos.y;
    const float bottom = nextPos.y + kHitboxHeight - epsilon;

    const int   topTile    = static_cast<int>(std::floor(top / tileSize));
    const int   bottomTile = static_cast<int>(std::floor(bottom / tileSize));

    if (deltaX > 0.0f) {
        const int tileX = static_cast<int>(std::floor(right / tileSize));
        for (int tileY = topTile; tileY <= bottomTile; ++tileY) {
            if (isSolidTile(level, tileX, tileY)) {
                nextPos.x   = tileX * tileSize - kHitboxWidth;
                _velocity.x = 0.0f;
                break;
            }
        }
    } else {
        const int tileX = static_cast<int>(std::floor(left / tileSize));
        for (int tileY = topTile; tileY <= bottomTile; ++tileY) {
            if (isSolidTile(level, tileX, tileY)) {
                nextPos.x   = (tileX + 1) * tileSize;
                _velocity.x = 0.0f;
                break;
            }
        }
    }
}

void Player::resolveVertical(const Level& level, dl::Vector2f& nextPos, float dtSeconds, float tileSize, float epsilon) {
    _onGround          = false;
    const float deltaY = _velocity.y * dtSeconds;
    if (deltaY == 0.0f) {
        return;
    }

    nextPos.y += deltaY;
    const float left   = nextPos.x;
    const float right  = nextPos.x + kHitboxWidth - epsilon;
    const float top    = nextPos.y;
    const float bottom = nextPos.y + kHitboxHeight - epsilon;

    const int   leftTile  = static_cast<int>(std::floor(left / tileSize));
    const int   rightTile = static_cast<int>(std::floor(right / tileSize));

    if (deltaY > 0.0f) {
        const int tileY = static_cast<int>(std::floor(bottom / tileSize));
        for (int tileX = leftTile; tileX <= rightTile; ++tileX) {
            if (isSolidTile(level, tileX, tileY)) {
                nextPos.y   = tileY * tileSize - kHitboxHeight;
                _velocity.y = 0.0f;
                _onGround   = true;
                break;
            }
        }
    } else {
        const int tileY = static_cast<int>(std::floor(top / tileSize));
        for (int tileX = leftTile; tileX <= rightTile; ++tileX) {
            if (isSolidTile(level, tileX, tileY)) {
                nextPos.y   = (tileY + 1) * tileSize;
                _velocity.y = 0.0f;
                break;
            }
        }
    }
}

void Player::probeGround(const Level& level, const dl::Vector2f& nextPos, float tileSize, float epsilon) {
    if (_onGround) {
        return;
    }

    const float left   = nextPos.x;
    const float right  = nextPos.x + kHitboxWidth - epsilon;
    const float bottom = nextPos.y + kHitboxHeight + epsilon;

    const int   leftTile  = static_cast<int>(std::floor(left / tileSize));
    const int   rightTile = static_cast<int>(std::floor(right / tileSize));
    const int   tileY     = static_cast<int>(std::floor(bottom / tileSize));

    for (int tileX = leftTile; tileX <= rightTile; ++tileX) {
        if (isSolidTile(level, tileX, tileY)) {
            _onGround = true;
            break;
        }
    }
}

void Player::handleHazards(const Level& level, float tileSize, float epsilon) {
    const float inset  = 2.0f;
    const float left   = _position.x + inset;
    const float right  = _position.x + kHitboxWidth - inset;
    const float top    = _position.y;
    const float bottom = _position.y + kHitboxHeight - inset;

    const int   leftTile   = static_cast<int>(std::floor(left / tileSize));
    const int   rightTile  = static_cast<int>(std::floor(right / tileSize));
    const int   topTile    = static_cast<int>(std::floor(top / tileSize));
    const int   bottomTile = static_cast<int>(std::floor(bottom / tileSize));

    for (int tileY = topTile; tileY <= bottomTile; ++tileY) {
        for (int tileX = leftTile; tileX <= rightTile; ++tileX) {
            if (tileX < 0 || tileY < 0) {
                continue;
            }
            if (level.getTile(static_cast<std::size_t>(tileX), static_cast<std::size_t>(tileY)) == Level::Tile::Saw) {
                const auto         spawnPoint = level.getSpawnPoint();
                const dl::Vector2f spawnPos   = spawnPoint.has_value() ? dl::Vector2f(spawnPoint->x * tileSize, spawnPoint->y * tileSize) : dl::Vector2f(tileSize, tileSize);
                _position                     = spawnPos;
                _velocity                     = dl::Vector2f(0.0f, 0.0f);
                _onGround                     = false;
                _moveDir                      = 0;
                _jumpQueued                   = false;
                return;
            }
        }
    }
}

void Player::updateAnimation(float dtSeconds) {
    const bool isMoving = _moveDir != 0;
    if (!_onGround) {
        _animTimer = 0.0f;
        setAnimationFrame(AnimFrame::Jump);
    } else if (isMoving) {
        _animTimer += dtSeconds;
        if (_animTimer >= kWalkFrameSeconds) {
            _animTimer = 0.0f;
            if (_currentFrame == AnimFrame::Walk1) {
                setAnimationFrame(AnimFrame::Walk2);
            } else {
                setAnimationFrame(AnimFrame::Walk1);
            }
        }
    } else {
        _animTimer = 0.0f;
        setAnimationFrame(AnimFrame::Idle);
    }
}

void Player::syncSprite() {
    if (_sprite) {
        _sprite->setPosition(_position);
    }
}

dl::FloatRect Player::getBounds() const {
    return dl::FloatRect(_position.x, _position.y, kHitboxWidth, kHitboxHeight);
}

dl::Sprite* Player::getSprite() {
    return _sprite.get();
}

const dl::Sprite* Player::getSprite() const {
    return _sprite.get();
}
