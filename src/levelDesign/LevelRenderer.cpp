/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelRenderer
*/

#include "LevelRenderer.hpp"

LevelRenderer::LevelRenderer(const Level& level)
    : _level(level),
      _startFlagAnimation(2, .5f),    // start flag: 2 frames, 0.5s per frame
      _endFlagAnimation(2, .5f) {     // end flag: 2 frames, 0.5s per frame
}

void LevelRenderer::initialize() {
    // Load sprite sheets
    _stoneTileSheet.loadFromFile("romfs:/assets/level/stone_tile.t3x");
    _startFlagSheet.loadFromFile("romfs:/assets/level/start_flag.t3x");
    _endFlagSheet.loadFromFile("romfs:/assets/level/end_flag.t3x");
    _sawbladeSheet.loadFromFile("romfs:/assets/level/sawblade.t3x");

    // Pre-allocate all tile sprites
    createTileSprites();
}

void LevelRenderer::createTileSprites() {
    _tileSprites.clear();
    constexpr float tileSize = 16.0f;

    // Iterate through level and create sprites for each non-empty tile
    for (std::size_t y = 0; y < _level.getHeight(); ++y) {
        for (std::size_t x = 0; x < _level.getWidth(); ++x) {
            Level::Tile tile = _level.getTile(x, y);

            if (tile != Level::Tile::Empty) {
                TileSprite tileSprite;
                tileSprite.position = dl::Vector2f(
                    static_cast<float>(x) * tileSize,
                    static_cast<float>(y) * tileSize
                );
                tileSprite.type = tile;

                // Load appropriate sprite based on tile type
                switch (tile) {
                    case Level::Tile::Block:
                        tileSprite.sprite.loadFromSpriteSheet(_stoneTileSheet, 0);
                        break;
                    case Level::Tile::Saw:
                        tileSprite.sprite.loadFromSpriteSheet(_sawbladeSheet, 0);
                        break;
                    case Level::Tile::Spawn:
                        tileSprite.sprite.loadFromSpriteSheet(_startFlagSheet, 0);
                        break;
                    case Level::Tile::Objective:
                        tileSprite.sprite.loadFromSpriteSheet(_endFlagSheet, 0);
                        break;
                    default:
                        break;
                }

                _tileSprites.push_back(tileSprite);
            }
        }
    }
}

void LevelRenderer::update(float dt) {
    // Update flag animations
    _startFlagAnimation.update(dt);
    _endFlagAnimation.update(dt);

    size_t startFlagFrame = _startFlagAnimation.getCurrentFrame();
    size_t endFlagFrame = _endFlagAnimation.getCurrentFrame();

    // Update animated sprites
    for (auto& tileSprite : _tileSprites) {
        switch (tileSprite.type) {
            case Level::Tile::Spawn:
                tileSprite.sprite.loadFromSpriteSheet(_startFlagSheet, startFlagFrame);
                break;
            case Level::Tile::Objective:
                tileSprite.sprite.loadFromSpriteSheet(_endFlagSheet, endFlagFrame);
                break;
            default:
                break;
        }
    }
}

void LevelRenderer::render(dl::RenderWindow& window) {
    // Draw all pre-allocated tiles
    for (auto& tileSprite : _tileSprites) {
        tileSprite.sprite.setPosition(tileSprite.position);
        window.draw(tileSprite.sprite);
    }
}
