#pragma once
#include <raylib.h>

class Laser {
public:
  Laser(Vector2 position, int speed);
  void update();
  void draw();
  bool active;
  Rectangle get_rect();

private:
  Vector2 position;
  int speed;
};