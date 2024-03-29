#include "ship.hpp"
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <string>

Ship::Ship() {
  image = LoadTexture("Graphics/spaceship.png");
  position.x = (750 - image.width) / 2.0f;
  position.y = 700 - image.height - 100;
  last_fire_time = 0.0;
  std::cout << "Ship constructor: ( " << std::to_string(position.x) << ", "
            << std::to_string(position.y) << " )" << std::endl;
  // laser_sound = LoadSound("Sounds/laser.ogg");
};

Ship::~Ship() {
  UnloadTexture(image);
  // UnloadSound(laser_sound);
};

void Ship::draw() { DrawTextureV(image, position, WHITE); };

void Ship::move_left() {
  position.x -= 7;
  if (position.x < 25) {
    position.x = 25;
  }
};

void Ship::move_right() {
  position.x += 7;
  if (position.x > 750 - image.width - 25) {
    position.x = 750 - image.width - 25;
  }
};

void Ship::fire_laser() {
  if (GetTime() - last_fire_time >= 0.35) {
    lasers.push_back(
        Laser({position.x + image.width / 2.0f - 2, position.y}, -6));
    last_fire_time = GetTime();
    // PlaySound(laser_sound);
  };
};

Rectangle Ship::get_rect() {
  return {position.x, position.y, float(image.width), float(image.height)};
};

void Ship::reset() {
  position.x = float(750 - image.width) / 2;
  position.y = 700 - image.height - 100;
  lasers.clear();
};