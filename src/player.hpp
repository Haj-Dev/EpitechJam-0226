#include <Dualie/Dualie.hpp>

class player {
  public:
    //player();
    //~player();
    dl::Vector2f getPos() {
        return (_pos);
    }
    void setPos(dl::Vector2f newPos) {
        _pos = newPos;
    }
    dl::Vector2f getVel() {
        return (_vel);
    }
    void setVel(dl::Vector2f newVel) {
        _pos = newVel;
    }

  private:
    dl::Vector2f _pos;
    dl::Vector2f _vel;
};