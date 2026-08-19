#pragma once

#include "raylib.h"

#include "Content.hpp"

const int MAX_HEALTH = 5;
const float DASH_SPEED = 1000.0f;
const float DASH_CD = 2.0f;
const float MAXSPEED = 200.0f;
const float TURN_FACTOR = 3.5;
const float ACCEL = 300.0f;
const float FRICTION = 100.0f;

struct Player {
  Player();

  void update();
  void draw() const;
  void reset();

  bool canShoot() const;
  void setPosition(const Vector2 &pos);

  // bike controls stuff
  Vector2 m_velocity;
  float m_facingAngle;

  int m_health;
  Vector2 m_position;
  float m_speed;
  float m_shootAngle;
  RotiKind m_held;
  Rectangle m_hitbox;
  double m_lastFired;
  float m_iFramesTime = 0.0f;
  Vector2 m_dashDir = {0, 0};
  float m_dashTime = 0.0f;
  float m_dashCooldown = 0.0f;
};
