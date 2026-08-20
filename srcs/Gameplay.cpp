#include "Gameplay.hpp"
#include "Content.hpp"
#include "Player.hpp"

#include "raylib.h"
// so that raymath can be used without triggering -Werror
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

#include <algorithm>

GameState Gameplay::update(Assets &asset) {
  if (m_winningTime > 0.0f) {
    m_winningTime -= GetFrameTime();
    if (m_winningTime <= 0.0f)
      return WON;

    return PLAYING;
  }
  // nothing updates if paused
  if (IsKeyPressed(KEY_P)) {
    m_gamePaused = !m_gamePaused;
  }
  if (m_gamePaused)
    return PLAYING;
  // -------------------------

  m_elapsed += GetFrameTime();
  if (m_messageTime > 0.0f)
    m_messageTime -= GetFrameTime();

  // spawn enemies
  while (m_nextSpawnIndex < m_enemyCount &&
         m_events[m_nextSpawnIndex].time <= m_elapsed) {
    SpawnEvent event = KSTAGES[m_stage].spawns[m_nextSpawnIndex];
    spawnEnemy(event.type);
    m_nextSpawnIndex++;
  }

  m_player.update(asset);

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    if (Vector2Length(m_player.m_velocity) <= 0.1f) {
      m_message = "Keep moving to shoot!";
      m_messageTime = 1.0f;
    } else {
      if (m_player.canShoot()) {
        Vector2 dir = {cosf(m_player.m_shootAngle),
                       sinf(m_player.m_shootAngle)};
        Vector2 pos = Vector2Add(m_player.m_position, Vector2Scale(dir, 20.0f));
        m_projectiles.push_back((Projectile){pos, dir, m_player.m_held, true,
                                             K_ROTI[m_player.m_held].timeLeft});
        PlaySound(asset.fxShoot);
        m_player.m_lastFired = GetTime();
      }
    }
  }

  // debug: spawn enemy on mouse pos
  // if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
  //   spawnEnemy(GetMousePosition());
  // }

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
    if (e.m_hurtTime > 0.0f)
      e.m_hurtTime -= GetFrameTime();
    Vector2 directionToPlayer = m_player.m_position - e.m_position;
    e.setPosition(e.m_position +
                  Vector2Scale(Vector2Normalize(directionToPlayer),
                               e.m_speed * GetFrameTime()));
  }

  // Projectiles hit enemy
  for (auto &p : m_projectiles) {
    if (p.m_isFriendly) {
      for (auto &e : m_enemies) {
        if (CheckCollisionCircleRec(p.m_position, 16, e.m_hitBox)) {
          e.m_hurtTime = 0.10f;
          e.m_health -= K_ROTI[p.m_kind].damage;
          if (e.m_health <= 0.0f) {
            e.m_alive = false;
          }
          p.m_alive = false;
          break;
        }
      }
    }
  }

  for (auto &e : m_enemies) {
    if (m_player.m_iFramesTime <= 0 &&
        CheckCollisionRecs(e.m_hitBox, m_player.m_hitbox)) {
      m_player.m_health -= 1;
      // getting hit and knocked back is like being forced to dash in opposite
      // direction
      m_player.m_dashTime = 0.1f;
      m_player.m_dashDir = Vector2Normalize(m_player.m_position - e.m_position);
      m_player.m_iFramesTime = 1.0f;
      m_player.m_velocity = {0.0f, 0.0f};
      PlaySound(asset.fxHurt);
      break;
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

  if (m_nextSpawnIndex == m_enemyCount && m_enemies.empty()) {
    m_winningTime = 2.0f;
    PlaySound(asset.fxWin);
  }
  if (m_player.m_health <= 0)
    return DIED;

  return PLAYING;
}

void Gameplay::draw(Assets &asset) const {
  m_player.draw(asset.texPlayer);

  for (auto &p : m_projectiles) {
    Rectangle src = {0.0f, 0.0f, (float)asset.texRoti.width,
                     (float)asset.texRoti.height};
    DrawTexturePro(asset.texRoti, src, {p.m_position.x, p.m_position.y, 32, 32},
                   {16.0f, 16.0f}, 0.0f, WHITE);
    // DEBUG
    // DrawCircleLines(p.m_position.x, p.m_position.y, 16.0f, RED);
  }

  for (auto &e : m_enemies) {
    // DrawRectangle(e.m_position.x, e.m_position.y, 32, 32, RED);

    Rectangle src = asset.getRectFromID(K_ENEMY[e.m_type].texId);
    Rectangle dst = {e.m_position.x, e.m_position.y, 32, 32};
    DrawTexturePro(asset.tilemap, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);
    if (e.m_hurtTime > 0.0f) {
      BeginBlendMode(BLEND_ADDITIVE);
      DrawTexturePro(asset.tilemap, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);
      EndBlendMode();
    }
  }

  if (m_messageTime > 0.0f)
    DrawText(m_message.c_str(), 20, 20, 20, BLACK);

  // Draw player health left
  for (int i = 0; i < m_player.m_health; i++) {
    DrawCircle(20 + (i * 40), GetScreenHeight() - 30, 15, RED);
  }

  if (m_gamePaused) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(DARKGRAY, 0.7));
    DrawText("GAMEPAUSED", 200, GetScreenHeight() / 2, 20, BLACK);
  }

  if (m_winningTime > 0.0f) {
    DrawTexturePro(
        asset.texFlag,
        {0, 0, (float)asset.texFlag.width, (float)asset.texFlag.height},
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0.0f,
        Fade(WHITE, 0.5f));
  }
}

void Gameplay::reset() {
  m_player.reset();
  m_projectiles.clear();
  m_enemies.clear();
  m_stage = 0;
  m_spawnSide = 0;
  m_enemyCount = KSTAGES[m_stage].enemyCount;
  m_events = KSTAGES[m_stage].spawns;
  m_elapsed = 0;
  m_nextSpawnIndex = 0;
  m_gamePaused = false;
  m_message = "Launch roti canai to feed customers.\n"
              "Don't drive into the customers or the wall.\n"
              "W: Accelerate  S: Brake\nA/D: Steer\n"
              "LClick: shoot roti";
  m_messageTime = 5.0f;
  m_winningTime = 0.0f;
}

void Gameplay::spawnEnemy(EnemyType type) {
  Vector2 spawnLocation;
  switch (m_spawnSide) {
  case 0:
    spawnLocation = {0.0f, (float)GetRandomValue(10, GetScreenHeight() - 10)};
    break;
  case 1:
    spawnLocation = {(float)GetScreenWidth(),
                     (float)GetRandomValue(10, GetScreenHeight())};
    break;
  case 2:
    spawnLocation = {(float)GetRandomValue(10, GetScreenWidth() - 10), 0.0f};
    break;
  case 3:
    spawnLocation = {(float)GetRandomValue(10, GetScreenHeight()),
                     (float)GetScreenHeight()};
    break;
  default:
    break;
  }
  m_enemies.push_back(Enemy{spawnLocation, type});
  m_spawnSide = (m_spawnSide + 1) % 4;
}

Enemy::Enemy(const Vector2 &pos, EnemyType type)
    : m_health(K_ENEMY[type].health), m_speed(K_ENEMY[type].speed),
      m_alive(true), m_type(type), m_hurtTime(0.0f) {
  setPosition(pos);
}

void Enemy::setPosition(const Vector2 &pos) {
  m_position = pos;
  m_hitBox = {pos.x, pos.y, 32, 32};
}

// void Gameplay::spawnProjectile(const Vector2 &pos, const Vector2 &velocity,
//                                bool isFriend) {
//   m_projectiles.push_back((Projectile){pos, velocity, isFriend, 200.0f});
// }
