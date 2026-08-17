#pragma once

#include "raylib.h"
#include <vector>

struct Player {
  Player() : m_position({400.0f, 300.0f}) {}

  Vector2 m_position;
  float m_speed;
};

struct Enemy {
  Enemy();

  Vector2 m_position;
};

struct Projectile {
  Vector2 position;
  Vector2 speed;
  bool isFriendly;
};

class Gameplay {
public:
  void Update();
  void Draw();

private:
  Player m_player;
  std::vector<Enemy> m_enemies;
};
