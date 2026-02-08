#pragma once

#include <Dualie/Dualie.hpp>
#include "Interfaces/IScene.hpp"
#include <string>

class game {
  public:
    game();
    ~game();
    void            runGame();
    bool            isWindowOpen();
    void            loadLevel(const std::string& levelPath);
    void            loadMenu();

    dl::TextBuffer& getTextBuffer() {
        return _sharedTextBuffer;
    }
    dl::SpriteSheet& getButtonSheet() {
        return _buttonSheet;
    }
    dl::SpriteSheet& getBackgroundBottom() {
        return _backgroundBottom;
    }
    dl::SpriteSheet& getBackgroundTop() {
        return _backgroundTop;
    }
    dl::Music& getLevelMusic() {
        return _levelMusic;
    }
    dl::Music& getMenuMusic() {
        return _menuMusic;
    }

  private:
    enum class PendingScene {
        None,
        Menu,
        Level
    };

    dl::RenderWindow        _window;
    dl::Clock               _clock;
    dl::Music               _levelMusic;
    dl::Music               _menuMusic;
    std::unique_ptr<IScene> _scene;
    PendingScene            _pendingScene;
    std::string             _pendingLevelPath;

    dl::TextBuffer          _sharedTextBuffer{4096};
    dl::SpriteSheet         _buttonSheet;
    dl::SpriteSheet         _backgroundBottom;
    dl::SpriteSheet         _backgroundTop;
};