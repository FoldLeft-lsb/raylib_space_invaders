#pragma once
#include <raylib.h>

class Alien {
public:
  Alien(int type, Vector2 position);
  static Texture2D alienImages[3];
  int type;
  Vector2 position;
  static void unload_images();
  void draw();
  void update(int direction);
  int get_type();
  Rectangle get_rect();
};