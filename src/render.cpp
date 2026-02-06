#include "render.hpp"

render::render() {}

render::~render() {}

void render::renderGame() {

    //perhaps put top and bottom screen in their respective functions
    window.clear(dl::TOP_SCREEN, dl::Color(0, 100, 0));
    window.display();

    window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 100, 0));
    window.display();
}

bool render::isWindowOpen() {
    return (window.isOpen());
}