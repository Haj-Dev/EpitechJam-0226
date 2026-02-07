/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** IScene
*/

#pragma once

#include <Dualie/Dualie.hpp>

class IScene {
  public:
    virtual ~IScene() = default;

    virtual void onEnter()                        = 0;
    virtual void onExit()                         = 0;
    virtual void update(float dt)                 = 0;
    virtual void render(dl::RenderWindow& window) = 0;
};
