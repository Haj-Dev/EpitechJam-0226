#include "game.hpp"
#include "menuScene/MenuScene.hpp"

#include <Dualie/System/Input.hpp>

game::game() {
    _music.loadFromFile("romfs:/SuperJamGirl.opus");
    _music.setLooping(true);
    _music.play();
    _scene = std::make_unique<MenuScene>();
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
        _scene->update(1 / 60);
        _scene->render(_window);
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}