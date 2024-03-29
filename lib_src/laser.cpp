#include "laser.hpp"
#include <raylib.h>

Laser::Laser(Vector2 position, int speed) {
  this->position = position;
  this->speed = speed;
  active = true;
};

void Laser::draw() {
  if (active) {
    DrawRectangle(position.x, position.y, 4, 15, Color{243, 216, 63, 255});
  };
};

void Laser::update() {
  position.y += speed;
  if (active) {
    if (position.y > GetScreenHeight() - 100 || position.y < 25) {
      active = false;
    }
  }
};

Rectangle Laser::get_rect() { return {position.x, position.y, 4, 15}; };