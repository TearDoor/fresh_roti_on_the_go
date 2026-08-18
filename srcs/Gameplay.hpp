#pragma once

#include "raylib.h"
#include <vector>

struct Player {
  Player() : m_position({400.0f, 300.0f}), m_speed(60.0f) {}

  void Update();
  void Draw() const;

  Vector2 m_position;
  float m_speed;
  float m_angle;
  double m_attackSpeed = 20.0f;
  double m_lastFired = GetTime();
};

struct Enemy {
  Enemy();

  Vector2 m_position;
};

struct Projectile {
  Vector2 m_position;
  Vector2 m_velocity;
  bool m_isFriendly;
  float m_speed;
};

class Gameplay {
public:
  void Update();
  void Draw();

  void spawnProjectile(const Vector2 &pos, const Vector2 &velocity,
                       bool isFriend);

private:
  Player m_player;
  std::vector<Enemy> m_enemies;
  std::vector<Projectile> m_projectiles;
};
