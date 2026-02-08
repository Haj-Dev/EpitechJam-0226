#include "game.hpp"

#include <Dualie/System/Input.hpp>

game::game() {
    _music.loadFromFile("romfs:/SuperJamGirl.opus");
    _music.setLooping(true);
    _music.play();
    _player.setPosition(dl::Vector2f(120.0f, 80.0f));
    _player.setGroundY(200.0f);
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

        _player.handleInput(dl::Input::isKeyHeld(dl::Input::LEFT), dl::Input::isKeyHeld(dl::Input::RIGHT), dl::Input::isKeyPressed(dl::Input::A));
        _player.update(kDeltaSeconds);

        _window.clear(dl::TOP_SCREEN, dl::Color(0, 0, 0));
        if (auto* sprite = _player.getSprite()) {
            _window.draw(*sprite);
        }
        _window.display();

        _window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 0, 0));
        _window.display();
    }
}

bool game::isWindowOpen() {
    return (_window.isOpen());
}