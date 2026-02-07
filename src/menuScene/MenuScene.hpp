/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** IScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
#include "../Interfaces/AScene.hpp"

class MenuScene : virtual public AScene {
  public:
    virtual ~MenuScene() = default;

    void onEnter() override;
    void onExit() override;
    //void handleInput() override;
    void update(float dt) override;
    void render(dl::RenderWindow window) override;
};
