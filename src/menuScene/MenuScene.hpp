/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** IScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
#include "../Interfaces/AScene.hpp"

class game;

class MenuScene : virtual public AScene {
  public:
    virtual ~MenuScene() = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt, game* gamePtr) override;
    void render(dl::RenderWindow& window) override;

  private:
    dl::SpriteSheet _background_bottom;
    dl::Sprite      _background_bottom_sprite;
    dl::SpriteSheet _background_top;
    dl::Sprite      _background_top_sprite;
};
