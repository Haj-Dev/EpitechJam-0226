#pragma once

#include <Dualie/Dualie.hpp>

class render {
  public:
    render();
    ~render();
    void renderGame();

    bool isWindowOpen();

  private:
    dl::RenderWindow window;
};