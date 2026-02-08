#include "MenuScene.hpp"

#include <array>

void MenuScene::onEnter() {
    _background_bottom.loadFromFile("romfs:/assets/ui/background_lower.t3x");
    _background_bottom_sprite.loadFromSpriteSheet(_background_bottom, 0);
    _background_bottom_sprite.setPosition(dl::Vector2f(0, 0));
    _background_top.loadFromFile("romfs:/assets/ui/background_upper.t3x");
    _background_top_sprite.loadFromSpriteSheet(_background_top, 0);
    _background_top_sprite.setPosition(dl::Vector2f(0, 0));

    _button_sheet.loadFromFile("romfs:/assets/ui/button.t3x");
    _levelButtons.clear();

    const dl::Vector2f               buttonSize(120.0f, 90.0f);
    const float                      columnSpacing = 20.0f;
    const float                      rowSpacing    = 12.0f;
    const float                      startX        = 30.0f;
    const float                      startY        = 24.0f;

    const std::array<std::string, 4> labels = {"Level 1", "Level 2", "Level 3", "Level 4"};

    const std::array<std::string, 4> paths = {"romfs:/levels/level1.txt", "romfs:/levels/level2.txt", "romfs:/levels/level3.txt", "romfs:/levels/level4.txt"};

    for (std::size_t i = 0; i < labels.size(); ++i) {
        Button      button(_button_sheet, 0, buttonSize, _textBuffer, labels[i]);
        const int   column = static_cast<int>(i % 2);
        const int   row    = static_cast<int>(i / 2);
        const float x      = startX + column * (buttonSize.x + columnSpacing);
        const float y      = startY + row * (buttonSize.y + rowSpacing);
        button.setPosition(dl::Vector2f(x, y));
        button.onClick = nullptr;
        _levelButtons.push_back(std::move(button));
    }
}

void MenuScene::onExit() {
    _levelButtons.clear();
}

void MenuScene::update(float dt) {
    (void)dt;
    for (auto& button : _levelButtons) {
        button.update();
    }
}

void MenuScene::render(dl::RenderWindow& window) {
    window.clear(dl::TOP_SCREEN, dl::Color(0, 100, 0));
    window.draw(_background_top_sprite);
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 100, 0));
    window.draw(_background_bottom_sprite);
    for (auto& button : _levelButtons) {
        window.draw(button);
    }
    window.display();
}
