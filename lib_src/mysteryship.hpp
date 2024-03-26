#pragma once
#include <raylib.h>

class MysteryShip {
public:
  MysteryShip();
  ~MysteryShip();
  void update();
  void draw();
  void spawn();
  Rectangle get_rect();
  bool alive;

private:
  Vector2 position;
  Texture2D image;
  int speed;
};