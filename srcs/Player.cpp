#include "Player.hpp"
#include "raylib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

Player::Player()
    : m_health(MAX_HEALTH), m_position({0, 0}), m_speed(100.0f), m_angle(0.0f),
      m_held(CANAI), m_hitbox({0, 0, 32, 3}), m_lastFired(-1) {}

void Player::draw() const {

  Vector2 localTop = {+20, 0};
  Vector2 localBackLeft = {-10, -10};
  Vector2 localBackRight = {-10, 10};

  // rotate the points then move it to player position
  Vector2 top = Vector2Add(m_position, Vector2Rotate(localTop, m_angle));
  Vector2 backLeft =
      Vector2Add(m_position, Vector2Rotate(localBackLeft, m_angle));
  Vector2 backRight =
      Vector2Add(m_position, Vector2Rotate(localBackRight, m_angle));

  DrawTriangle(top, backLeft, backRight, BLUE);
  DrawRectangleLinesEx(m_hitbox, 1, RED);
}

void Player::update() {
  // mouseDirectionVector = mousePos - playerPos
  // angle (starting from +x axis) = atan2(dir.y, dir.x)
  // thats why our angle start as facing right
  m_angle = atan2(GetMouseY() - m_position.y, GetMouseX() - m_position.x);

  if (m_iFramesTime > 0)
    m_iFramesTime -= GetFrameTime();
  // in 'dashing' state (dashing, knocked back)
  if (m_dashTime > 0) {
    setPosition(m_position +
                Vector2Scale(m_dashDir, DASH_SPEED * GetFrameTime()));
    m_dashTime -= GetFrameTime();
    return;
  }

  // WASD movement
  Vector2 inputDir{0, 0};
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
    setPosition(m_position + Vector2Scale(inputDir, m_speed * GetFrameTime()));
  }

  // SPACE to dash
  if (IsKeyPressed(KEY_SPACE)) {
    m_dashTime = 0.10f;
    m_iFramesTime = 0.15f;
    m_dashDir = inputDir;
  }
}

void Player::reset() {
  m_health = MAX_HEALTH;
  setPosition((Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f});
  m_held = CANAI;
  m_lastFired = -1;
}

bool Player::canShoot() const {
  // 10 atkspd fires once per second, 20 twice, 5 once per 2 secs
  double delay = 10.0f / K_ROTI[m_held].fireRate;
  return (m_lastFired < 0 || GetTime() - m_lastFired >= delay);
}

void Player::setPosition(const Vector2 &pos) {
  m_position = pos;
  m_hitbox = {pos.x - 16, pos.y - 16, 32, 32};
}
