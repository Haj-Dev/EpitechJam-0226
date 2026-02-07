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
        while (_clock.getElapsedTime().asSeconds() < 1 / 60) {
            //wait
        }
        _clock.restart();

        _scene->update(1 / 60);

        _scene->render(_window);
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}