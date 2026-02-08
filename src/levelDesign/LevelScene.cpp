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
#include <cmath>

LevelScene::LevelScene(game* gameInstance, const std::string& levelPath) : _game(gameInstance), _levelPath(levelPath) {}

void LevelScene::onEnter() {
    _game->getLevelMusic().play();

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

    _stoneTileSpriteSheet.loadFromFile("romfs:/assets/level/stone_tile.t3x");
    _startFlagSpriteSheet.loadFromFile("romfs:/assets/level/start_flag.t3x");
    _endFlagSpriteSheet.loadFromFile("romfs:/assets/level/end_flag.t3x");
    _sawBaldeSpriteSheet.loadFromFile("romfs:/assets/level/sawblade.t3x");

    for (size_t w = 0; w < _level->getWidth(); w++) {
        for (size_t h = 0; h < _level->getHeight(); h++) {
            switch (_level->getTile(w, h)) {
                case Level::Tile::Block: _spriteMap[w][h].loadFromSpriteSheet(_stoneTileSpriteSheet, 0); break;
                case Level::Tile::Spawn: _spriteMap[w][h].loadFromSpriteSheet(_startFlagSpriteSheet, 0); break;
                case Level::Tile::Objective: _spriteMap[w][h].loadFromSpriteSheet(_endFlagSpriteSheet, 0); break;
                case Level::Tile::Saw: _spriteMap[w][h].loadFromSpriteSheet(_sawBaldeSpriteSheet, 0); break;
                case Level::Tile::Empty: break;
            }
        }
    }

    _playerspriteSheet.loadFromFile("romfs:/assets/jam_girl/jam_girl.t3x");
    _playersprite.loadFromSpriteSheet(_playerspriteSheet, 0);
}

void LevelScene::onExit() {
    _game->getLevelMusic().stop();
    _menuButton.reset();
    _restartButton.reset();
    _level.reset();
    _game->getMenuMusic().play();
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

    if (isPlayerOnObjective()) {
        _game->loadMenu();
        return;
    }
}

bool LevelScene::isPlayerOnObjective() const {
    if (!_level) {
        return false;
    }

    const dl::FloatRect bounds  = _player.getBounds();
    const float         epsilon = 0.001f;
    const float         left    = bounds.left;
    const float         right   = bounds.left + bounds.width - epsilon;
    const float         top     = bounds.top;
    const float         bottom  = bounds.top + bounds.height - epsilon;

    const int           leftTile   = static_cast<int>(std::floor(left / Level::kTileSize));
    const int           rightTile  = static_cast<int>(std::floor(right / Level::kTileSize));
    const int           topTile    = static_cast<int>(std::floor(top / Level::kTileSize));
    const int           bottomTile = static_cast<int>(std::floor(bottom / Level::kTileSize));

    for (int tileY = topTile; tileY <= bottomTile; ++tileY) {
        for (int tileX = leftTile; tileX <= rightTile; ++tileX) {
            if (tileX < 0 || tileY < 0) {
                continue;
            }
            if (_level->getTile(static_cast<std::size_t>(tileX), static_cast<std::size_t>(tileY)) == Level::Tile::Objective) {
                return true;
            }
        }
    }

    return false;
}

void LevelScene::render(dl::RenderWindow& window) {
    window.clear(dl::TOP_SCREEN, dl::Color(100, 100, 200));
    float x = std::clamp(_player.getPosition().x, (float)System::TOP_WIDTH / 2, (float)_level->getWidth() * 16 - System::TOP_WIDTH / 2);
    float y = std::clamp(_player.getPosition().y, (float)System::TOP_HEIGHT / 2, (float)_level->getHeight() * 16 - System::TOP_HEIGHT / 2);
    for (size_t w = (size_t)std::max((int)x / 16 - System::TOP_WIDTH / 16 / 2 - 1, 0); w < (size_t)x / 16 + System::TOP_WIDTH / 16 / 2 + 1; w++) {
        for (size_t h = (size_t)std::max((int)y / 16 - System::TOP_HEIGHT / 16 / 2 - 1, 0); h < (size_t)y / 16 + System::TOP_HEIGHT / 16 / 2 + 1; h++) {
            if (_level->getTile(w, h) != Level::Tile::Empty) {
                _spriteMap[w][h].setPosition(dl::Vector2f(w * 16 - x + System::TOP_WIDTH / 2, h * 16 - y + System::TOP_HEIGHT / 2));
                window.draw(_spriteMap[w][h]);
            }
        }
    }
    int screenPlayerY = System::TOP_HEIGHT / 2;
    if (_player.getPosition().y < System::TOP_HEIGHT / 2) {
        screenPlayerY = _player.getPosition().y;
    }
    if (_player.getPosition().y > _level->getHeight() * 16 - System::TOP_HEIGHT / 2) {
        screenPlayerY = _player.getPosition().y - (_level->getHeight() * 16 - System::TOP_HEIGHT);
    }
    int screenPlayerX = System::TOP_WIDTH / 2;
    if (_player.getPosition().x < System::TOP_WIDTH / 2) {
        screenPlayerX = _player.getPosition().x;
    }
    if (_player.getPosition().x > _level->getWidth() * 16 - System::TOP_WIDTH / 2) {
        screenPlayerX = _player.getPosition().x - (_level->getWidth() * 16 - System::TOP_WIDTH);
    }
    _playersprite.setPosition(dl::Vector2f(screenPlayerX, screenPlayerY));
    window.draw(_playersprite);
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(100, 100, 100));
    window.draw(_background_bottom_sprite);
    window.draw(*_menuButton);
    window.draw(*_restartButton);
    window.display();
}
