#include "MenuScene.hpp"
#include "../game.hpp"
#include "../levelDesign/LevelScene.hpp"

void MenuScene::onEnter() {
    _background_bottom.loadFromFile("romfs:/assets/ui/background_lower.t3x");
    _background_bottom_sprite.loadFromSpriteSheet(_background_bottom, 0);
    _background_bottom_sprite.setPosition(dl::Vector2f(0, 0));
    _background_top.loadFromFile("romfs:/assets/ui/background_upper.t3x");
    _background_top_sprite.loadFromSpriteSheet(_background_top, 0);
    _background_top_sprite.setPosition(dl::Vector2f(0, 0));
}

void MenuScene::onExit() {}

void MenuScene::update(float dt, game* gamePtr) {
    // Check if START button is pressed
    if (dl::Input::isKeyPressed(dl::Input::START)) {
        // Transition to Level 1 - Death Platform Level
        gamePtr->changeScene(std::make_unique<LevelScene>("romfs:/levelDesign/PersitentDeathLevel.txt"));
    }
}

void MenuScene::render(dl::RenderWindow& window) {
    window.clear(dl::TOP_SCREEN, dl::Color(0, 100, 0));
    window.draw(_background_top_sprite);
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 100, 0));
    window.draw(_background_bottom_sprite);
    window.display();
}
