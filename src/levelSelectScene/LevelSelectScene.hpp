/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelSelectScene
*/

#pragma once

#include <Dualie/Dualie.hpp>
#include "../Interfaces/AScene.hpp"
#include <string>
#include <vector>

class game;

class LevelSelectScene : virtual public AScene {
  public:
    virtual ~LevelSelectScene() = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt, game* gamePtr) override;
    void render(dl::RenderWindow& window) override;

  private:
    struct LevelEntry {
        std::string name;
        std::string path;
    };

    std::vector<LevelEntry> _levels;
    size_t _selectedIndex;

    dl::TextBuffer _textBuffer;
    dl::Text _titleText;
    std::vector<dl::Text*> _levelTexts;

  public:
    LevelSelectScene();
};
