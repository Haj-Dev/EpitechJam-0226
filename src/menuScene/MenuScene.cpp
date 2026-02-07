#include "MenuScene.hpp"

void MenuScene::onEnter() {
    _background_bottom.loadFromFile("romfs:/assets/ui/background_lower.t3x");
    _background_bottom_sprite.loadFromSpriteSheet(_background_bottom, 0);
    _background_bottom_sprite.setPosition(dl::Vector2f(0, 0));
}

void MenuScene::onExit() {}

void MenuScene::update(float dt) {}

void MenuScene::render(dl::RenderWindow& window) {
    window.clear(dl::TOP_SCREEN, dl::Color(0, 100, 0));
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 100, 0));
    window.draw(_background_bottom_sprite);
    window.display();
}
