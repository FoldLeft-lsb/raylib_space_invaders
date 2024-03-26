#include "game.hpp"
#include "alien.hpp"
#include "obstacle.hpp"
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>

Game::Game() {
  // music = LoadMusicStream("Sounds/music.ogg");
  explosion_sound = LoadSound("Sounds/explosion.ogg");
  // PlayMusicStream(music);
  init_game();
};

Game::~Game() {
  Alien::unload_images();
  // UnloadMusicStream(music);
  UnloadSound(explosion_sound);
};

void Game::init_game() {
  obstacles = create_obstacles();
  aliens = create_aliens();
  aliens_direction = 1;
  time_last_alien_shoot = 0;
  mysteryship_last_spawn = 0.0;
  mysteryship_spawn_interval = GetRandomValue(10, 20);
  lives = 3;
  run = true;
  score = 0;
  high_score = load_highscore_from_file();
};

void Game::reset() {
  spaceship.reset();
  aliens.clear();
  alienLasers.clear();
  obstacles.clear();
};

void Game::update() {
  if (run) {
    double currentTime = GetTime();
    if (currentTime - mysteryship_last_spawn > mysteryship_spawn_interval) {
      mysteryship.spawn();
      mysteryship_last_spawn = GetTime();
      mysteryship_spawn_interval = GetRandomValue(10, 20);
    }

    for (auto &laser : spaceship.lasers) {
      laser.update();
    }
    move_aliens();
    alien_shoot_laser();
    for (auto &laser : alienLasers) {
      laser.update();
    }
    delete_inactive_lasers();
    mysteryship.update();
    check_collisions();
  } else {
    if (IsKeyPressed(KEY_R)) {
      reset();
      init_game();
    }
  }
};

void Game::draw() {
  spaceship.draw();
  for (auto &laser : spaceship.lasers) {
    laser.draw();
  }
  for (auto &obstacle : obstacles) {
    obstacle.draw();
  }
  for (auto &alien : aliens) {
    alien.draw();
  }
  for (auto &laser : alienLasers) {
    laser.draw();
  }
  mysteryship.draw();
};

void Game::handle_input() {
  if (run) {
    if (IsKeyDown(KEY_A)) {
      spaceship.move_left();
    }
    if (IsKeyDown(KEY_D)) {
      spaceship.move_right();
    }
    if (IsKeyDown(KEY_SPACE)) {
      spaceship.fire_laser();
    }
  }
};

void Game::delete_inactive_lasers() {
  for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
    if (!it->active) {
      it = spaceship.lasers.erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = alienLasers.begin(); it != alienLasers.end();) {
    if (!it->active) {
      it = alienLasers.erase(it);
    } else {
      ++it;
    }
  }
};

std::vector<Obstacle> Game::create_obstacles() {
  int obstacleWidth = Obstacle::grid[0].size() * 3;
  float gap = float(GetScreenWidth() - (4 * obstacleWidth)) / 5;

  for (int i = 0; i < 4; i++) {
    float offsetX = (i + 1) * gap + i * obstacleWidth;
    obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
  }
  return obstacles;
};

std::vector<Alien> Game::create_aliens() {
  std::vector<Alien> aliens;
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 11; col++) {
      int alienType;
      if (row == 0) {
        alienType = 3;
      } else if (row == 1 || row == 2) {
        alienType = 2;
      } else {
        alienType = 1;
      };
      float x = 75 + col * 55;
      float y = 110 + row * 55;
      aliens.push_back(Alien(alienType, {x, y}));
    }
  }
  return aliens;
};

void Game::move_aliens() {
  for (auto &alien : aliens) {
    if (alien.position.x + alien.alienImages[alien.type - 1].width >
        GetScreenWidth() - 25) {
      aliens_direction = -1;
      move_aliens_down(4);
    } else if (alien.position.x < 25) {
      aliens_direction = 1;
      move_aliens_down(4);
    };
    alien.update(aliens_direction);
  }
};

void Game::move_aliens_down(int distance) {
  for (auto &alien : aliens) {
    alien.position.y += distance;
  }
};

void Game::alien_shoot_laser() {
  double currentTime = GetTime();
  if (currentTime - time_last_alien_shoot >= alien_laser_shoot_interval &&
      !aliens.empty()) {

    int rIndex = GetRandomValue(0, aliens.size() - 1);
    Alien &alien = aliens[rIndex];
    alienLasers.push_back(Laser(
        {alien.position.x + float(alien.alienImages[alien.type - 1].width) / 2,
         alien.position.y + float(alien.alienImages[alien.type - 1].height)},
        6));
    time_last_alien_shoot = GetTime();
  }
};

void Game::check_collisions() {
  // Spaceship lasers
  for (auto &laser : spaceship.lasers) {
    // Aliens
    auto it = aliens.begin();
    while (it != aliens.end()) {
      if (CheckCollisionRecs(it->get_rect(), laser.get_rect())) {
        if (it->type == 1) {
          score += 100;
        } else if (it->type == 2) {
          score += 200;
        } else if (it->type == 3) {
          score += 300;
        }
        check_high_score();
        it = aliens.erase(it);
        laser.active = false;
        PlaySound(explosion_sound);
      } else {
        ++it;
      }
    }
    // Obstacles
    for (auto &obstacle : obstacles) {
      auto it = obstacle.blocks.begin();
      while (it != obstacle.blocks.end()) {
        if (CheckCollisionRecs(it->get_rect(), laser.get_rect())) {
          it = obstacle.blocks.erase(it);
          laser.active = false;
        } else {
          ++it;
        }
      }
    }
    // Mystery ship
    if (CheckCollisionRecs(mysteryship.get_rect(), laser.get_rect())) {
      mysteryship.alive = false;
      laser.active = false;
      score += 500;
      check_high_score();
      PlaySound(explosion_sound);
    }
  }

  // Alien lasers
  for (auto &laser : alienLasers) {
    if (CheckCollisionRecs(laser.get_rect(), spaceship.get_rect())) {
      laser.active = false;
      lives--;
      if (lives == 0) {
        game_over();
      }
    }

    for (auto &obstacle : obstacles) {
      auto it = obstacle.blocks.begin();
      while (it != obstacle.blocks.end()) {
        if (CheckCollisionRecs(it->get_rect(), laser.get_rect())) {
          it = obstacle.blocks.erase(it);
          laser.active = false;
        } else {
          ++it;
        }
      }
    }
  }

  // Aliens collide with obstacles
  for (auto &alien : aliens) {
    for (auto &obstacle : obstacles) {
      auto it = obstacle.blocks.begin();
      while (it != obstacle.blocks.end()) {
        if (CheckCollisionRecs(it->get_rect(), alien.get_rect())) {
          it = obstacle.blocks.erase(it);
        } else {
          it++;
        }
      }
    }

    if (CheckCollisionRecs(alien.get_rect(), spaceship.get_rect())) {
      game_over();
    }
  }
};

void Game::check_high_score() {
  if (score > high_score) {
    high_score = score;
    save_highscore_to_file(high_score);
  }
};

void Game::game_over() {
  std::cout << "Game Over!" << std::endl;
  run = false;
};

void Game::save_highscore_to_file(int high_score) {
  std::ofstream highscoreFile("highscore.txt");
  if (highscoreFile.is_open()) {
    highscoreFile << high_score;
    highscoreFile.close();
  } else {
    std::cerr << "Failed to save highscore to file" << std::endl;
  }
};

int Game::load_highscore_from_file() {
  int loaded_highscore = 0;
  std::ifstream highscoreFile("highscore.txt");
  if (highscoreFile.is_open()) {
    highscoreFile >> loaded_highscore;
    highscoreFile.close();
  } else {
    std::cerr << "Failed to load highscore from file" << std::endl;
  }
  return loaded_highscore;
};
