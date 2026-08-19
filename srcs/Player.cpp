#include "Player.hpp"
#include "raylib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

Player::Player()
    : m_health(MAX_HEALTH), m_position({0, 0}), m_speed(100.0f),
      m_shootAngle(0.0f), m_held(CANAI), m_lastFired(-1) {}

void Player::update() {
  // mouseDirectionVector = mousePos - playerPos
  // angle (starting from +x axis) = atan2(dir.y, dir.x)
  // thats why our angle start as facing right
  m_shootAngle = atan2(GetMouseY() - m_position.y, GetMouseX() - m_position.x);
  float deltaTime = GetFrameTime();

  Vector2 finalPos = m_position;

  if (m_iFramesTime > 0)
    m_iFramesTime -= deltaTime;
  if (m_dashCooldown > 0) {
    m_dashCooldown -= GetFrameTime();
  }

  if (m_dashTime > 0) {
    finalPos =
        m_position + Vector2Scale(m_dashDir, DASH_SPEED * GetFrameTime());
    m_dashTime -= GetFrameTime();
  } else {
    float speed = Vector2Length(m_velocity);

    if (IsKeyDown(KEY_A)) {
      m_facingAngle -= 3.5 * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
      m_facingAngle += 3.5 * deltaTime;
    }

    Vector2 forward = {cos(m_facingAngle), sin(m_facingAngle)};

    if (IsKeyDown(KEY_W)) {
      speed += ACCEL * deltaTime;
    } else if (IsKeyDown(KEY_S)) {
      speed -= ACCEL * deltaTime;
    } else {
      if (speed > 0.1) {
        speed -= FRICTION * deltaTime;
      } else {
        m_velocity = {0, 0};
      }
    }
    speed = Clamp(speed, 0, MAXSPEED);
    m_velocity = forward * speed;

    finalPos = m_position + (m_velocity * deltaTime);
  }

  finalPos.x = Clamp(finalPos.x, 0.0f, GetScreenWidth());
  finalPos.y = Clamp(finalPos.y, 0.0f, GetScreenHeight());
  setPosition(finalPos);

  applyWallCollision();

  return;
}

void Player::draw() const {

  Vector2 localTop = {+20, 0};
  Vector2 localBackLeft = {-10, -10};
  Vector2 localBackRight = {-10, 10};

  // rotate the points then move it to player position
  Vector2 top = Vector2Add(m_position, Vector2Rotate(localTop, m_facingAngle));
  Vector2 backLeft =
      Vector2Add(m_position, Vector2Rotate(localBackLeft, m_facingAngle));
  Vector2 backRight =
      Vector2Add(m_position, Vector2Rotate(localBackRight, m_facingAngle));

  DrawTriangle(top, backLeft, backRight, BLUE);
  // debug hitbox
  DrawRectangleLinesEx(m_hitbox, 1, RED);
}

void Player::reset() {
  m_health = MAX_HEALTH;
  setPosition((Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f});
  m_held = CANAI;
  m_facingAngle = 0.0f;
  m_velocity = {0, 0};
  m_lastFired = -1;
  m_dashTime = 0.0f;
  m_iFramesTime = 0.0f;
  m_dashCooldown = 0.0f;
}

bool Player::canShoot() const {
  // 10 atkspd fires once per second, 20 twice, 5 once per 2 secs
  double delay = 10.0f / K_ROTI[m_held].fireRate;
  return (m_lastFired < 0 || GetTime() - m_lastFired >= delay);
}

void Player::applyWallCollision() {
  if (m_hitbox.x <= 0.0f || m_hitbox.x + m_hitbox.width >= GetScreenWidth() ||
      m_hitbox.y <= 0.0f || m_hitbox.y + m_hitbox.height >= GetScreenHeight()) {
    m_health -= 1;
    m_dashDir = Vector2Normalize(m_velocity) * -1;
    m_velocity = {0.0f, 0.0f};
    m_dashTime = 0.05f;
    m_iFramesTime = 1.0f;
  }
}

void Player::setPosition(const Vector2 &pos) {
  m_position = pos;
  m_hitbox = {pos.x - 16, pos.y - 16, 32, 32};
}
