#include "levelDesign.hpp"

#include <algorithm>
#include <fstream>
#include <string>

Level::Level() = default;

Level::Level(const std::string& path) {
    loadFromFile(path);
}

bool Level::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        _tiles.clear();
        _spawnPoint.reset();
        _objectivePoint.reset();
        return false;
    }

    std::vector<std::vector<Tile>> rows;
    std::string                    line;

    _spawnPoint.reset();
    _objectivePoint.reset();

    while (std::getline(file, line)) {
        std::vector<Tile> row;
        const std::size_t readWidth = std::min(line.size(), kLevelWidth);
        row.reserve(readWidth);
        const std::size_t y = rows.size();

        for (std::size_t x = 0; x < readWidth; ++x) {
            const char ch = line[x];
            switch (ch) {
                case '0': row.push_back(Tile::Block); break;
                case '1': row.push_back(Tile::Saw); break;
                case '2':
                    row.push_back(Tile::Spawn);
                    _spawnPoint = dl::Vector2i(static_cast<int>(x), static_cast<int>(y));
                    break;
                case '3':
                    row.push_back(Tile::Objective);
                    _objectivePoint = dl::Vector2i(static_cast<int>(x), static_cast<int>(y));
                    break;
                default: row.push_back(Tile::Empty); break;
            }
        }
        rows.push_back(std::move(row));
    }

    _tiles = std::move(rows);
    return true;
}

std::size_t Level::getWidth() const {
    return kLevelWidth;
}

std::size_t Level::getHeight() const {
    return _tiles.size();
}

Level::Tile Level::getTile(std::size_t x, std::size_t y) const {
    if (y >= _tiles.size()) {
        return Tile::Empty;
    }
    const auto& row = _tiles[y];
    if (x >= row.size()) {
        return Tile::Empty;
    }
    return row[x];
}

std::optional<dl::Vector2i> Level::getSpawnPoint() const {
    return _spawnPoint;
}

std::optional<dl::Vector2i> Level::getObjectivePoint() const {
    return _objectivePoint;
}
