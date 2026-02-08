/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
#include "../Interfaces/AScene.hpp"
#include "levelDesign.hpp"
#include "LevelRenderer.hpp"
#include "AnimationController.hpp"

class game;

class LevelScene : virtual public AScene {
  public:
    explicit LevelScene(const std::string& levelPath);
    virtual ~LevelScene() = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt, game* gamePtr) override;
    void render(dl::RenderWindow& window) override;

  private:
    // Level data and rendering
    Level           _level;
    LevelRenderer   _levelRenderer;

    // Player rendering
    dl::SpriteSheet _playerSheet;
    dl::Sprite      _playerSprite;
    AnimationController _playerAnimation;

    // Player physics
    dl::Vector2f    _playerPos;
    dl::Vector2f    _playerVelocity;
    bool            _playerOnGround;
    dl::Vector2f    _spawnPoint;

    // Death system - dead bodies become platforms
    std::vector<dl::Vector2f> _deadBodies;
    std::vector<dl::Sprite>   _deadBodySprites;

    // Pause menu
    bool            _paused;
    size_t          _pauseMenuIndex;
    dl::TextBuffer  _pauseTextBuffer;
    dl::Text        _pauseTitleText;
    dl::Text        _pauseResumeText;
    dl::Text        _pauseLevelSelectText;
    dl::Text        _pauseQuitText;
    static constexpr size_t PAUSE_MENU_COUNT = 3;

    // Helper methods
    void respawnPlayer();
    bool checkDeadBodyCollision(const dl::Vector2f& pos, float& deadBodyY);
    bool checkSawCollision(const dl::Vector2f& pos);
    bool checkBlockCollisionBelow(const dl::Vector2f& pos, float& blockY);
    bool checkBlockCollisionAbove(const dl::Vector2f& pos, float& blockBottomY);
    bool checkBlockCollisionHorizontal(const dl::Vector2f& pos, float velocityX, float& correctedX);
    bool checkDeadBodyCollisionHorizontal(const dl::Vector2f& pos, float velocityX, float& correctedX);
    void addDeadBody(const dl::Vector2f& pos);

    // Game constants
    static constexpr float GRAVITY = 20.0f;
    static constexpr float MOVE_SPEED = 10.0f;
    static constexpr float JUMP_SPEED = -50.0f;
    static constexpr float DEATH_Y = 240.0f; // Fall off bottom of screen
    static constexpr size_t MAX_DEAD_BODIES = 5;
};
