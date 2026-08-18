#include "Gameplay.hpp"
#include "Player.hpp"
#include "raylib.h"
// so that raymath can be used without triggering -Werror
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

#include <algorithm>

GameState Gameplay::update() {
  m_player.update();

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    if (m_player.canShoot()) {
      Vector2 dir = {cosf(m_player.m_angle), sinf(m_player.m_angle)};
      Vector2 pos = Vector2Add(m_player.m_position, Vector2Scale(dir, 20.0f));
      m_projectiles.push_back((Projectile){pos, dir, m_player.m_held, true,
                                           K_ROTI[m_player.m_held].timeLeft});
      m_player.m_lastFired = GetTime();
    }
  }

  // debug: spawn enemy on mouse pos
  if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
    spawnEnemy(GetMousePosition());
  }

  // projectile movement and lifetime
  for (auto &p : m_projectiles) {
    p.m_position.x += p.m_velocity.x * K_ROTI[p.m_kind].speed * GetFrameTime();
    p.m_position.y += p.m_velocity.y * K_ROTI[p.m_kind].speed * GetFrameTime();
    p.timeLeft -= GetFrameTime();
    if (p.timeLeft <= 0.0f) {
      p.m_alive = false;
    }
  }

  // enemy movement (homing onto player)
  for (auto &e : m_enemies) {
    Vector2 directionToPlayer = m_player.m_position - e.m_position;
    e.setPosition(e.m_position +
                  Vector2Scale(Vector2Normalize(directionToPlayer),
                               75.0f * GetFrameTime()));
  }

  for (auto &p : m_projectiles) {
    if (p.m_isFriendly) {
      for (auto &e : m_enemies) {
        if (CheckCollisionCircleRec(p.m_position, 10, e.m_hitBox)) {
          // TODO: flashing animation
          e.health -= K_ROTI[p.m_kind].damage;
          if (e.health <= 0.0f) {
            e.m_alive = false;
          }
          p.m_alive = false;
          break;
        }
      }
    }
  }

  for (auto &e : m_enemies) {
    if (CheckCollisionRecs(e.m_hitBox, m_player.m_hitbox)) {
      m_player.m_health -= 1;
      if (m_player.m_health == 0) {
        return DIED;
      }
    }
  }

  // objects cleanup
  m_projectiles.erase(
      std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                     [](const Projectile &p) { return !p.m_alive; }),
      m_projectiles.end());
  m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                                 [](const Enemy &e) { return !e.m_alive; }),
                  m_enemies.end());
  return PLAYING;
}

void Gameplay::draw() const {
  m_player.draw();

  for (auto &p : m_projectiles) {
    DrawCircleV(p.m_position, 10, YELLOW);
  }

  for (auto &e : m_enemies) {
    DrawRectangle(e.m_position.x, e.m_position.y, 32, 32, RED);
  }
}

void Gameplay::reset() {
  m_player.reset();
  m_projectiles.clear();
  m_enemies.clear();
}

void Gameplay::spawnEnemy(const Vector2 &pos) {
  m_enemies.push_back(Enemy{pos});
}

void Enemy::setPosition(const Vector2 &pos) {
  m_position = pos;
  m_hitBox = {pos.x, pos.y, 32, 32};
}

// void Gameplay::spawnProjectile(const Vector2 &pos, const Vector2 &velocity,
//                                bool isFriend) {
//   m_projectiles.push_back((Projectile){pos, velocity, isFriend, 200.0f});
// }
