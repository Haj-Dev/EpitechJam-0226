#pragma once

#include "logic.hpp"
#include "render.hpp"
#include "player.hpp"

class game {
  public:
    game();
    ~game();
    void runGame();

  private:
    logic  _logic;
    render _render;
    player _player;
};