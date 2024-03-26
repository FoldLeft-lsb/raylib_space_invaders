#pragma once
#include "laser.hpp"
#include <raylib.h>
#include <vector>

class Ship {
public:
  Ship();
  ~Ship();
  void draw();
  void move_left();
  void move_right();
  void fire_laser();
  void reset();
  Rectangle get_rect();
  std::vector<Laser> lasers;

private:
  Texture2D image;
  Vector2 position;
  double last_fire_time;
  Sound laser_sound;
};