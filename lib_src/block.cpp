#include "block.hpp"
#include <raylib.h>

Block::Block(Vector2 position) { this->position = position; };

void Block::draw() {
  DrawRectangle(position.x, position.y, 3, 3, Color{243, 216, 63, 255});
};

Rectangle Block::get_rect() { return {position.x, position.y, 3, 3}; };