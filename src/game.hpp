#pragma once

#include <Dualie/Dualie.hpp>
#include "Interfaces/IScene.hpp"
#include <string>

class game {
  public:
    game();
    ~game();
    void runGame();
    bool isWindowOpen();
    void loadLevel(const std::string& levelPath);
    void loadMenu();

  private:
    enum class PendingScene {
        None,
        Menu,
        Level
    };

    dl::RenderWindow        _window;
    dl::Clock               _clock;
    dl::Music               _music;
    std::unique_ptr<IScene> _scene;
    PendingScene            _pendingScene;
    std::string             _pendingLevelPath;
};