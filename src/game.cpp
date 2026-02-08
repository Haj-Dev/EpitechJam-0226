#include "game.hpp"
#include "menuScene/MenuScene.hpp"
#include "levelDesign/LevelScene.hpp"

#include <Dualie/System/Input.hpp>

game::game() : _pendingScene(PendingScene::None) {
    _menuMusic.loadFromFile("romfs:/main-menu.opus");
    _menuMusic.setLooping(true);
    _levelMusic.loadFromFile("romfs:/level.opus");
    _levelMusic.setLooping(true);

    _buttonSheet.loadFromFile("romfs:/assets/ui/button.t3x");
    _backgroundBottom.loadFromFile("romfs:/assets/ui/background_lower.t3x");
    _backgroundTop.loadFromFile("romfs:/assets/ui/background_upper.t3x");

    _scene = std::make_unique<MenuScene>(this);
    _scene->onEnter();
}

game::~game() {}

void game::runGame() {
    _menuMusic.play();
    constexpr float kDeltaSeconds = 1.0f / 60.0f;
    _clock.restart();
    while (isWindowOpen()) {
        _clock.restart();
        dl::Input::updateInput();

        if (_pendingScene != PendingScene::None) {
            _scene->onExit();
            _scene.reset();
            if (_pendingScene == PendingScene::Menu) {
                _scene = std::make_unique<MenuScene>(this);
            } else {
                _scene = std::make_unique<LevelScene>(this, _pendingLevelPath);
            }
            _scene->onEnter();
            _pendingScene = PendingScene::None;
        }

        _scene->update(1.0f / 60.0f);
        _scene->render(_window);
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}

void game::loadLevel(const std::string& levelPath) {
    _pendingLevelPath = levelPath;
    _pendingScene     = PendingScene::Level;
}

void game::loadMenu() {
    _pendingScene = PendingScene::Menu;
}