/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** Player
*/

#include "Player.hpp"

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

void Player::update(float dtSeconds) {
    _velocity.x = static_cast<float>(_moveDir) * kMoveSpeed;

    const bool onGround = _position.y >= _groundY;
    if (onGround && _jumpQueued) {
        _velocity.y = -kJumpSpeed;
    }
    _jumpQueued = false;

    _velocity.x += _acceleration.x * dtSeconds;
    _velocity.y += (kGravity + _acceleration.y) * dtSeconds;
    _position.x += _velocity.x * dtSeconds;
    _position.y += _velocity.y * dtSeconds;
    if (_position.y > _groundY) {
        _position.y = _groundY;
        if (_velocity.y > 0.0f) {
            _velocity.y = 0.0f;
        }
    }

    const bool isMoving   = _moveDir != 0;
    const bool isOnGround = _position.y >= _groundY;
    if (!isOnGround) {
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
    if (_sprite) {
        _sprite->setPosition(_position);
    }
}

dl::Sprite* Player::getSprite() {
    return _sprite.get();
}

const dl::Sprite* Player::getSprite() const {
    return _sprite.get();
}
