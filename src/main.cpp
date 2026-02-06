#include <Dualie/Dualie.hpp>
#include "System.hpp"

int main() {
    dl::RenderWindow window;

    while (window.isOpen()) {
        window.clear(dl::TOP_SCREEN, dl::Color(0, 0, 0));
        window.display();

        window.clear(dl::BOTTOM_SCREEN, dl::Color(0, 0, 0));
        window.display();
    }

    return 0;
}
