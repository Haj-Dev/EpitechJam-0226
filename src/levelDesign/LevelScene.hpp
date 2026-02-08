/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelScene
*/

#pragma once

#include <Dualie/Dualie.hpp>

#include <cstddef>

#include "../Interfaces/AScene.hpp"
#include "../ui/Button.hpp"
#include "levelDesign.hpp"
#include "../Player/Player.hpp"

class game;

class LevelScene : virtual public AScene {
  public:
    LevelScene(game* gameInstance, const std::string& levelPath);
    virtual ~LevelScene() = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void render(dl::RenderWindow& window) override;

  private:
    bool                         isPlayerOnObjective() const;
    void                         updateTileAnimations(float dtSeconds);

    game*                        _game;
    std::unique_ptr<Level>       _level;
    std::string                  _levelPath;
    std::unique_ptr<Button>      _menuButton;
    std::unique_ptr<Button>      _restartButton;
    dl::Sprite                   _background_bottom_sprite;
    Player                       _player;
    dl::SpriteSheet              _stoneTileSpriteSheet;
    dl::SpriteSheet              _startFlagSpriteSheet;
    dl::SpriteSheet              _endFlagSpriteSheet;
    dl::SpriteSheet              _sawBaldeSpriteSheet;
    dl::Sprite                   _spriteMap[100][100]; //TEMP
    float                        _sawAnimClock  = 0.0f;
    float                        _flagAnimClock = 0.0f;
    std::size_t                  _sawFrame      = 0;
    std::size_t                  _flagFrame     = 0;

    static constexpr float       kSawSecondsPerFrame  = 0.5f;
    static constexpr float       kFlagSecondsPerFrame = 1.5f;
    static constexpr std::size_t kSawFrameCount       = 2;
    static constexpr std::size_t kFlagFrameCount      = 2;
};
