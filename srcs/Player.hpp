#pragma once

#include "raylib.h"

enum RotiKind { CANAI = 0, TELUR, PISANG, BOM, PLANTA };

struct RotiStats {
  int damage;
  float speed;
  double fireRate;
  float timeLeft;
};

const RotiStats K_ROTI[] = {
    {10, 1000.0f, 20.0f, 1.0f}, // canai

};

const int MAX_HEALTH = 5;
const float DASH_SPEED = 1000.0f;

struct Player {
  Player();

  void update();
  void draw() const;
  void reset();

  bool canShoot() const;
  void setPosition(const Vector2 &pos);

  int m_health;
  Vector2 m_position;
  float m_speed;
  float m_angle;
  RotiKind m_held;
  Rectangle m_hitbox;
  double m_lastFired;
  float m_iFramesTime = 0.0f;
  Vector2 m_dashDir = {0, 0};
  float m_dashTime = 0.0f;
};
