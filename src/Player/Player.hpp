/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** Player
*/

#pragma once

#include <Dualie/Dualie.hpp>

#include <cstddef>
#include <memory>

class Player {
  public:
    enum class AnimFrame : std::size_t {
        Idle  = 0,
        Walk1 = 1,
        Walk2 = 2,
        Jump  = 3,
    };

    Player();
    ~Player();

    void                setAnimationFrame(AnimFrame frame);

    const dl::Vector2f& getPosition() const;
    void                setPosition(const dl::Vector2f& position);
    void                move(const dl::Vector2f& offset);

    const dl::Vector2f& getVelocity() const;
    void                setVelocity(const dl::Vector2f& velocity);

    const dl::Vector2f& getAcceleration() const;
    void                setAcceleration(const dl::Vector2f& acceleration);

    void                setGroundY(float groundY);
    void                handleInput(bool moveLeft, bool moveRight, bool jumpPressed);

    void                update(float dtSeconds);

    dl::Sprite*         getSprite();
    const dl::Sprite*   getSprite() const;

  protected:
  private:
    std::unique_ptr<dl::Sprite> _sprite;
    dl::SpriteSheet             _spriteSheet;
    dl::Vector2f                _position;
    dl::Vector2f                _velocity;
    dl::Vector2f                _acceleration;
    AnimFrame                   _currentFrame = AnimFrame::Idle;
    float                       _groundY      = 200.0f;
    int                         _moveDir      = 0;
    bool                        _jumpQueued   = false;
    float                       _animTimer    = 0.0f;

    static constexpr float      kGravity          = 30.0f;
    static constexpr float      kMoveSpeed        = 30.0f;
    static constexpr float      kJumpSpeed        = 50.0f;
    static constexpr float      kWalkFrameSeconds = 0.25f;
};
