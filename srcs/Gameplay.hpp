#pragma once

#include "raylib.h"
#include <vector>

#include "Player.hpp"

enum GameState { PLAYING, DIED, WON };

struct Enemy {
  Enemy(const Vector2 &pos, EnemyType type);
  int m_health;
  float m_speed;
  Vector2 m_position;
  Rectangle m_hitBox;
  bool m_alive;

  void setPosition(const Vector2 &pos);
};

struct Projectile {
  Vector2 m_position;
  Vector2 m_velocity;
  RotiKind m_kind;
  bool m_isFriendly;
  float timeLeft;
  bool m_alive = true;
};

class Gameplay {
public:
  GameState update();
  void draw() const;
  void reset();

  void spawnEnemy(EnemyType type);
  void spawnProjectile(const Vector2 &pos, const Vector2 &velocity,
                       bool isFriend);

private:
  // game objects
  Player m_player;
  std::vector<Enemy> m_enemies;
  std::vector<Projectile> m_projectiles;

  // gameplay variables
  int m_enemyCount;
  const SpawnEvent *m_events;
  int m_stage;
  float m_elapsed;
  int m_spawnSide;
  int m_nextSpawnIndex;
  bool m_gamePaused;
};
