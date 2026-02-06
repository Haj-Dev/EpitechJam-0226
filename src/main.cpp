#include <Dualie/Dualie.hpp>

int main() {
    const int          TOP_WIDTH  = 400;
    const int          TOP_HEIGHT = 240;

    const int          BOTTOM_WIDTH  = 320;
    const int          BOTTOM_HEIGHT = 240;

    dl::RenderWindow   window;

    dl::RectangleShape rect;
    rect.setSize(dl::Vector2f(150, 150));
    rect.setFillColor(dl::Color::Red);
    rect.setOrigin(dl::Vector2f(75, 75));
    rect.setPosition(TOP_WIDTH / 2, TOP_HEIGHT / 2);

    dl::CircleShape circle;
    circle.setRadius(50);
    circle.setFillColor(dl::Color::Blue);
    circle.setOrigin(50, 50);
    circle.setPosition(BOTTOM_WIDTH / 2, BOTTOM_HEIGHT / 2);

    while (window.isOpen()) {

        window.clear(dl::TOP_SCREEN, dl::Color(200, 200, 200));
        window.draw(rect);
        window.display();

        window.clear(dl::BOTTOM_SCREEN, dl::Color(50, 0, 0));
        window.draw(circle);
        window.display();
    }

    return 0;
}
