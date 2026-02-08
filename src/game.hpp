#pragma once

#include <Dualie/Dualie.hpp>
#include "Interfaces/IScene.hpp"
#include "Player/Player.hpp"

class game {
  public:
    game();
    ~game();
    void runGame();
    bool isWindowOpen();

  private:
    dl::RenderWindow        _window;
    dl::Clock               _clock;
    dl::Music               _music;
    std::unique_ptr<IScene> _scene;
    Player                  _player;
};