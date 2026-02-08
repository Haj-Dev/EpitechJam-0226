/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelRenderer
*/

#pragma once
#include <Dualie/Dualie.hpp>
#include "levelDesign.hpp"
#include "AnimationController.hpp"
#include <vector>

class LevelRenderer {
public:
    explicit LevelRenderer(const Level& level);
    void initialize();
    void update(float dt);
    void render(dl::RenderWindow& window);

private:
    struct TileSprite {
        dl::Sprite sprite;
        dl::Vector2f position;
        Level::Tile type;
    };

    const Level& _level;
    dl::SpriteSheet _stoneTileSheet;
    dl::SpriteSheet _startFlagSheet;
    dl::SpriteSheet _endFlagSheet;
    dl::SpriteSheet _sawbladeSheet;
    std::vector<TileSprite> _tileSprites;
    AnimationController _startFlagAnimation;
    AnimationController _endFlagAnimation;

    void createTileSprites();
};
