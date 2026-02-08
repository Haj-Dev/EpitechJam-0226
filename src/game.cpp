#include "game.hpp"
#include "menuScene/MenuScene.hpp"

game::game() {
    _music.loadFromFile("romfs:/SuperJamGirl.opus");
    _music.setLooping(true);
    _music.play();
    _scene = std::make_unique<MenuScene>();
    _scene->onEnter();
}

game::~game() {}

void game::runGame() {

    _clock.restart();
    while (isWindowOpen()) {
        while (_clock.getElapsedTime().asSeconds() < 1.0f / 60.0f) {
            //wait
        }
        _clock.restart();

        _scene->update(1.0f / 60.0f, this);

        _scene->render(_window);
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}

void game::changeScene(std::unique_ptr<IScene> newScene) {
    if (_scene) {
        _scene->onExit();
    }
    _scene = std::move(newScene);
    if (_scene) {
        _scene->onEnter();
    }
}