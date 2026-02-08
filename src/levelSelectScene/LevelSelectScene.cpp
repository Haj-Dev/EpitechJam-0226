/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** LevelSelectScene
*/

#include "LevelSelectScene.hpp"
#include "../game.hpp"
#include "../menuScene/MenuScene.hpp"
#include "../levelDesign/LevelScene.hpp"

LevelSelectScene::LevelSelectScene()
    : _selectedIndex(0),
      _textBuffer(256),
      _titleText(_textBuffer) {
}

void LevelSelectScene::onEnter() {
    // Define available levels
    _levels.clear();
    _levels.push_back({"Level 1 - Persistent Death", "romfs:/levelDesign/PersitentDeathLevel.txt"});

    _selectedIndex = 0;

    // Setup title text
    _titleText.setString("Select Level");
    _titleText.setPosition(dl::Vector2f(120, 20));
    _titleText.setScale(dl::Vector2f(1.0f, 1.0f));
    _titleText.setColor(dl::Color(255, 255, 255));

    // Cleanup old level texts
    for (auto* text : _levelTexts) {
        delete text;
    }
    _levelTexts.clear();

    // Create text for each level
    for (size_t i = 0; i < _levels.size(); ++i) {
        auto* text = new dl::Text(_textBuffer);
        text->setString(_levels[i].name);
        text->setPosition(dl::Vector2f(40, 60 + static_cast<float>(i) * 30));
        text->setScale(dl::Vector2f(0.8f, 0.8f));
        text->setColor(dl::Color(200, 200, 200));
        _levelTexts.push_back(text);
    }
}

void LevelSelectScene::onExit() {
    for (auto* text : _levelTexts) {
        delete text;
    }
    _levelTexts.clear();
}

void LevelSelectScene::update(float dt, game* gamePtr) {
    // Navigate levels
    if (dl::Input::isKeyPressed(dl::Input::DUP) && _selectedIndex > 0) {
        _selectedIndex--;
    }
    if (dl::Input::isKeyPressed(dl::Input::DDOWN) && _selectedIndex < _levels.size() - 1) {
        _selectedIndex++;
    }

    // Update text colors based on selection
    for (size_t i = 0; i < _levelTexts.size(); ++i) {
        if (i == _selectedIndex) {
            _levelTexts[i]->setColor(dl::Color(255, 255, 0)); // Yellow for selected
        } else {
            _levelTexts[i]->setColor(dl::Color(200, 200, 200)); // Gray for others
        }
    }

    // Start selected level
    if (dl::Input::isKeyPressed(dl::Input::A)) {
        gamePtr->changeScene(std::make_unique<LevelScene>(_levels[_selectedIndex].path));
    }

    // Go back to main menu
    if (dl::Input::isKeyPressed(dl::Input::B)) {
        gamePtr->changeScene(std::make_unique<MenuScene>());
    }
}

void LevelSelectScene::render(dl::RenderWindow& window) {
    // Top screen - title
    window.clear(dl::TOP_SCREEN, dl::Color(30, 30, 60));
    window.draw(_titleText);
    window.display();

    // Bottom screen - level list
    window.clear(dl::BOTTOM_SCREEN, dl::Color(20, 20, 40));
    for (auto* text : _levelTexts) {
        window.draw(*text);
    }
    window.display();
}
