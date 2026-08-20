#include "Player.hpp"
#include "raylib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

Player::Player()
    : m_health(MAX_HEALTH), m_position({0, 0}), m_shootAngle(0.0f),
      m_held(CANAI), m_lastFired(-1) {}

void Player::update(Assets &asset) {
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

    if (IsKeyPressed(KEY_W))
      PlaySound(asset.fxMotor);
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

  if (m_iFramesTime <= 0)
    applyWallCollision();

  return;
}

void Player::draw(Texture2D texPlayer) const {
  Rectangle src = {0.0f, 0.0f, (float)texPlayer.width, (float)texPlayer.height};
  Rectangle dst = {m_position.x, m_position.y, 32, 32};

  Color tint = WHITE;
  // flash red when hurt/invicibility time
  if (m_iFramesTime > 0.0f)
    tint = (fmodf(m_iFramesTime, 0.12) < 0.06) ? RED : WHITE;
  DrawTexturePro(texPlayer, src, dst, {16.0, 16.0}, m_facingAngle * RAD2DEG,
                 tint);

  // debug hitbox
  // DrawRectangleLinesEx(m_hitbox, 1, RED);
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
