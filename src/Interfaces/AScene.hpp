/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** AbstractScene
*/

#pragma once

#include "IScene.hpp"

class AScene : public IScene {
  public:
    ~AScene() override = default;

    void onEnter() override                        = 0;
    void onExit() override                         = 0;
    void handleInput() override                    = 0;
    void update(float dt) override                 = 0;
    void render(dl::RenderWindow& window) override = 0;
};
