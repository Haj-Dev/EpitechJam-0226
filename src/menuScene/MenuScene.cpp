#include "MenuScene.hpp"

void MenuScene::onEnter() {}

void MenuScene::onExit() {}

void MenuScene::update(float dt) {}

void MenuScene::render(dl::RenderWindow window) {
    window.clear(dl::TOP_SCREEN, dl::Color(0, 100, 0));
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 100, 0));
    window.display();
}
