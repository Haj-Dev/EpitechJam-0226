#include "game.hpp"

game::game() {}

game::~game() {}

void game::runGame() {
    while (_render.isWindowOpen()) {
        //some sort of clock or time control logic here

        _logic.runLogicTick();

        _render.renderGame();
    }
}