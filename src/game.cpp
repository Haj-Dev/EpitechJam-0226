#include "game.hpp"
#include "menuScene/MenuScene.hpp"
#include "levelDesign/LevelScene.hpp"

#include <Dualie/System/Input.hpp>

game::game() : _pendingScene(PendingScene::None) {
    _music.loadFromFile("romfs:/SuperJamGirl.opus");
    _music.setLooping(true);
    _music.play();
    _scene = std::make_unique<MenuScene>(this);
    _scene->onEnter();
}

game::~game() {}

void game::runGame() {
    constexpr float kDeltaSeconds = 1.0f / 60.0f;
    _clock.restart();
    while (isWindowOpen()) {
        while (_clock.getElapsedTime().asSeconds() < kDeltaSeconds) {
            //wait
        }
        _clock.restart();
        dl::Input::updateInput();

        if (_pendingScene != PendingScene::None) {
            _scene->onExit();
            if (_pendingScene == PendingScene::Menu) {
                _scene = std::make_unique<MenuScene>(this);
            } else {
                _scene = std::make_unique<LevelScene>(this, _pendingLevelPath);
            }
            _scene->onEnter();
            _pendingScene = PendingScene::None;
        }

        _scene->update(1 / 60);
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