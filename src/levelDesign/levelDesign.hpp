/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** levelDesign
*/

#pragma once

#include <Dualie/Dualie.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

class Level {
  public:
    enum class Tile {
        Empty,
        Block,
        Saw,
        Spawn,
        Objective
    };

    Level();
    explicit Level(const std::string& path);

    bool                        loadFromFile(const std::string& path);

    std::size_t                 getWidth() const;
    std::size_t                 getHeight() const;
    Tile                        getTile(std::size_t x, std::size_t y) const;
    std::optional<dl::Vector2i> getSpawnPoint() const;
    std::optional<dl::Vector2i> getObjectivePoint() const;

  private:
    static constexpr std::size_t   kLevelWidth = 25;
    std::vector<std::vector<Tile>> _tiles;
    std::optional<dl::Vector2i>    _spawnPoint;
    std::optional<dl::Vector2i>    _objectivePoint;
};
