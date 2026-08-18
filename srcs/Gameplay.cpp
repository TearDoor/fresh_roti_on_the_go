#include "Gameplay.hpp"
#include "raylib.h"
// so that raymath can be used without triggering -Werror
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

void Player::Draw() const {

  Vector2 localTop = {+20, 0};
  Vector2 localBackLeft = {-10, -10};
  Vector2 localBackRight = {-10, 10};

  // rotate the points then move it to player position
  Vector2 top = Vector2Add(m_position, Vector2Rotate(localTop, angle));
  Vector2 backLeft =
      Vector2Add(m_position, Vector2Rotate(localBackLeft, angle));
  Vector2 backRight =
      Vector2Add(m_position, Vector2Rotate(localBackRight, angle));

  DrawTriangle(top, backLeft, backRight, BLUE);
}

void Player::Update() {
  // mouseDirectionVector = mousePos - playerPos
  // angle (starting from +x axis) = atan2(dir.y, dir.x)
  // thats why our angle start as facing right
  angle = atan2(GetMouseY() - m_position.y, GetMouseX() - m_position.x);

  Vector2 inputDir;
  if (IsKeyDown(KEY_W))
    inputDir.y -= 1;
  if (IsKeyDown(KEY_S))
    inputDir.y += 1;
  if (IsKeyDown(KEY_A))
    inputDir.x -= 1;
  if (IsKeyDown(KEY_D))
    inputDir.x += 1;

  if (inputDir.x != 0 || inputDir.y != 0) {
    inputDir = Vector2Normalize(inputDir);
    m_position = Vector2Add(m_position,
                            Vector2Scale(inputDir, m_speed * GetFrameTime()));
  }
}

void Gameplay::Update() {
  m_player.Update();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 dir = {cosf(m_player.angle), sinf(m_player.angle)};
    Vector2 pos = Vector2Add(m_player.m_position, Vector2Scale(dir, 20.0f));
    m_projectiles.push_back((Projectile){pos, dir, true, 200.0f});
  }

  for (auto &p : m_projectiles) {
    p.m_position.x += p.m_velocity.x * p.m_speed * GetFrameTime();
    p.m_position.y += p.m_velocity.y * p.m_speed * GetFrameTime();
  }
  return;
}

void Gameplay::Draw() {
  m_player.Draw();

  for (auto &p : m_projectiles) {
    DrawCircleV(p.m_position, 10, YELLOW);
  }
}

// void Gameplay::spawnProjectile(const Vector2 &pos, const Vector2 &velocity,
//                                bool isFriend) {
//   m_projectiles.push_back((Projectile){pos, velocity, isFriend, 200.0f});
// }
