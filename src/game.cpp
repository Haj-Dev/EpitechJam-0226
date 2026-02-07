#include "game.hpp"
#include "menuScene/MenuScene.hpp"

game::game() {
    //_scene = new (MenuScene);
}

game::~game() {}

void game::runGame() {

    _clock.restart();
    while (isWindowOpen()) {
        while (_clock.getElapsedTime().asSeconds() < 1 / 60) {
            //wait
        }
        _clock.restart();

        //_scene.handleInput();

        //_scene->update(1 / 60);

        //_scene->render(_window);
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}