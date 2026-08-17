#pragma once

#include "raylib.h"
#include <vector>

struct Player {
  Player() : m_position({400.0f, 300.0f}), m_speed(60.0f) {}

  void Update();
  void Draw() const;

  Vector2 m_position;
  float m_speed;
  float angle;
};

struct Enemy {
  Enemy();

  Vector2 m_position;
};

struct Projectile {
  Vector2 m_position;
  Vector2 m_speed;
  bool m_isFriendly;
};

class Gameplay {
public:
  void Update();
  void Draw();

  void spawnProjectile();

private:
  Player m_player;
  std::vector<Enemy> m_enemies;
  std::vector<Projectile> m_projectiles;
};
