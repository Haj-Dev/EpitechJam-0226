/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** IScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../Interfaces/AScene.hpp"
#include "../levelDesign/levelDesign.hpp"
#include "../ui/Button.hpp"

class game;

class MenuScene : virtual public AScene {
  public:
    MenuScene(game* gameInstance);
    virtual ~MenuScene() = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void render(dl::RenderWindow& window) override;

  private:
    game*               _game;
    dl::Sprite          _background_bottom_sprite;
    dl::Sprite          _background_top_sprite;
    std::vector<Button> _levelButtons;
};
