/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
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
    bool                    isPlayerOnObjective() const;
    game*                   _game;
    std::unique_ptr<Level>  _level;
    std::string             _levelPath;
    std::unique_ptr<Button> _menuButton;
    std::unique_ptr<Button> _restartButton;
    dl::Sprite              _background_bottom_sprite;
    Player                  _player;
    dl::SpriteSheet         _stoneTileSpriteSheet;
    dl::SpriteSheet         _startFlagSpriteSheet;
    dl::SpriteSheet         _endFlagSpriteSheet;
    dl::SpriteSheet         _sawBaldeSpriteSheet;
    dl::Sprite              _spriteMap[100][100]; //TEMP
    dl::SpriteSheet         _playerspriteSheet;
    dl::Sprite              _playersprite;
};
