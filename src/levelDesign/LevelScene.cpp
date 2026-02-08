/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelScene
*/

#include "LevelScene.hpp"
#include "../game.hpp"
#include "../System.hpp"

#include <Dualie/Dualie.hpp>

LevelScene::LevelScene(game* gameInstance, const std::string& levelPath) : _game(gameInstance), _levelPath(levelPath) {}

void LevelScene::onEnter() {
    _level                        = std::make_unique<Level>(_levelPath);
    const auto         spawnPoint = _level->getSpawnPoint();
    const dl::Vector2f spawnPos =
        spawnPoint.has_value() ? dl::Vector2f(spawnPoint->x * Level::kTileSize, spawnPoint->y * Level::kTileSize) : dl::Vector2f(Level::kTileSize, Level::kTileSize);
    _player.setPosition(spawnPos);
    _player.setVelocity(dl::Vector2f(0.0f, 0.0f));

    const dl::Vector2f buttonSize(System::BUTTON_WIDTH, System::BUTTON_HEIGHT);
    _menuButton    = std::make_unique<Button>(_game->getButtonSheet(), 0, buttonSize, _game->getTextBuffer(), "Menu");
    _restartButton = std::make_unique<Button>(_game->getButtonSheet(), 0, buttonSize, _game->getTextBuffer(), "Restart");

    _menuButton->setPosition(dl::Vector2f(20.0f, 75.0f));
    _restartButton->setPosition(dl::Vector2f(180.0f, 75.0f));

    _menuButton->onClick    = [this]() { _game->loadMenu(); };
    _restartButton->onClick = [this]() { _game->loadLevel(_levelPath); };

    _background_bottom_sprite.loadFromSpriteSheet(_game->getBackgroundBottom(), 0);
    _background_bottom_sprite.setPosition(dl::Vector2f(0, 0));
}

void LevelScene::onExit() {
    _menuButton.reset();
    _restartButton.reset();
    _level.reset();
}

void LevelScene::update(float dt) {
    _menuButton->update();
    _restartButton->update();

    const bool moveLeft  = dl::Input::isKeyHeld(dl::Input::LEFT);
    const bool moveRight = dl::Input::isKeyHeld(dl::Input::RIGHT);
    const bool jump      = dl::Input::isKeyPressed(dl::Input::A);
    _player.handleInput(moveLeft, moveRight, jump);

    if (_level) {
        _player.update(dt, *_level);
    }
}

void LevelScene::render(dl::RenderWindow& window) {
    window.clear(dl::TOP_SCREEN, dl::Color(100, 100, 100));
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(100, 100, 100));
    window.draw(_background_bottom_sprite);
    window.draw(*_menuButton);
    window.draw(*_restartButton);
    window.display();
}
