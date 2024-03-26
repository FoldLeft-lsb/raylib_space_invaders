#pragma once
#include "alien.hpp"
#include "mysteryship.hpp"
#include "obstacle.hpp"
#include "ship.hpp"
#include <vector>

class Game {
public:
  Game();
  ~Game();
  void draw();
  void update();
  void handle_input();
  int lives;
  bool run;
  int score;
  int high_score;
  Music music;

private:
  void delete_inactive_lasers();
  std::vector<Obstacle> create_obstacles();
  std::vector<Alien> create_aliens();
  void move_aliens();
  void move_aliens_down(int distance);
  void alien_shoot_laser();
  void check_collisions();
  void game_over();
  void reset();
  void init_game();
  void check_high_score();
  void save_highscore_to_file(int high_score);
  int load_highscore_from_file();
  Ship spaceship;
  std::vector<Obstacle> obstacles;
  std::vector<Alien> aliens;
  int aliens_direction;
  std::vector<Laser> alienLasers;
  constexpr static float alien_laser_shoot_interval = 0.35;
  float time_last_alien_shoot;
  MysteryShip mysteryship;
  float mysteryship_spawn_interval;
  float mysteryship_last_spawn;
  Sound explosion_sound;
};