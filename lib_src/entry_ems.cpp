#include "entry.hpp"
#include "game.hpp"
#include <iostream>
#include <raylib.h>
#include <string>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

std::string left_pad_int_to_string(int number, int width) {
  std::string numberText = std::to_string(number);
  return numberText =
             std::string(width - numberText.length(), '0') + numberText;
};

Texture2D spaceshipImage;
Game game;

Color GREY = Color{29, 29, 27, 255};
Color YELLOW_2 = Color{243, 216, 63, 255};

int offset = 50;
const int vpWidth = 750;
const int vpHeight = 700;

void update_draw_frame() {
  // UpdateMusicStream(game.music);
  game.handle_input();
  game.update();

  BeginDrawing();
  ClearBackground(GREY);

  DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, YELLOW_2);
  DrawLineEx({25, 730}, {775, 730}, 3, YELLOW_2);

  DrawText(game.run ? "LEVEL 01" : "GAME OVER", 570, 740, 32, YELLOW_2);
  DrawText("SCORE", 50, 15, 32, YELLOW_2);
  std::string scoreText = left_pad_int_to_string(game.score, 5);
  DrawText(scoreText.c_str(), 50, 40, 32, YELLOW_2);

  DrawText("HIGH-SCORE", 570, 15, 32, YELLOW_2);
  std::string highScoreText = left_pad_int_to_string(game.high_score, 5);
  DrawText(highScoreText.c_str(), 655, 40, 32, YELLOW_2);

  float x = 50;
  for (int i = 0; i < game.lives; i++) {
    DrawTextureV(spaceshipImage, {x, 745}, WHITE);
    x += 50;
  }

  if (!game.run) {
    DrawText("PAUSED", 570, 640, 32, YELLOW_2);
  }

  game.draw();

  EndDrawing();
};

int loop() {
  spaceshipImage = LoadTexture("Graphics/spaceship.png");

  SetTargetFPS(60);
  InitWindow(vpWidth + offset, vpHeight + 2 * offset, "Space Invaders");
  // InitAudioDevice();

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(update_draw_frame, 0, 1);
#else
  std::cout << "GAME: Failed to load Platform correctly" << std::endl;
#endif

  CloseWindow();
  // CloseAudioDevice();

  return 0;
}