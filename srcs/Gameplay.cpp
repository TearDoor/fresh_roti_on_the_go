#include "Gameplay.hpp"
#include "raylib.h"
// so that raymath can be used without triggering -Werror
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

void Player::Draw() const {

  Vector2 localTip = {+20, 0};
  Vector2 localBackLeft = {-10, -10};
  Vector2 localBackRight = {-10, 10};

  Vector2 tip = Vector2Add(m_position, Vector2Rotate(localTip, angle));
  Vector2 backLeft =
      Vector2Add(m_position, Vector2Rotate(localBackLeft, angle));
  Vector2 backRight =
      Vector2Add(m_position, Vector2Rotate(localBackRight, angle));

  DrawTextEx(GetFontDefault(), TextFormat("[%i %i]", GetMouseX(), GetMouseY()),
             Vector2Add(GetMousePosition(), (Vector2){-44, -22}), 20, 2, BLACK);

  DrawTriangle(tip, backLeft, backRight, BLUE);
}

void Player::Update() {
  // mouseDirectionVector = mousePos - playerPos
  // angle (starting from +x axis) = atan2(dir.y, dir.x)
  // thats why our angle start as facing right
  angle = atan2(GetMouseY() - m_position.y, GetMouseX() - m_position.x);

  if (IsKeyDown(KEY_W)) {
    m_position.y -= m_speed * GetFrameTime();
  }
}

void Gameplay::Update() {
  m_player.Update();

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    spawnProjectile();
  }

  for (auto &p : m_projectiles) {
    p.m_position.x += p.m_speed.x * GetFrameTime();
    p.m_position.y += p.m_speed.y * GetFrameTime();
  }
  return;
}

void Gameplay::Draw() { m_player.Draw(); }

void Gameplay::spawnProjectile() {}
