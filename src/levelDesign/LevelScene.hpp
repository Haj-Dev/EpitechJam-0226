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
    game*                   _game;
    std::unique_ptr<Level>  _level;
    std::string             _levelPath;
    std::unique_ptr<Button> _menuButton;
    std::unique_ptr<Button> _restartButton;
    dl::Sprite              _background_bottom_sprite;
};
