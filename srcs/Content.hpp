#pragma once

// a file for presets

enum RotiKind { CANAI = 0, TELUR, PISANG, BOM, PLANTA };

struct RotiStats {
  int damage;
  float speed;
  double fireRate;
  float timeLeft;
};

inline const RotiStats K_ROTI[] = {
    {10, 500.0f, 10.0f, 1.0f}, // canai

};

enum EnemyType { BASIC, ELITE };

struct EnemyStats {
  int health;
  float speed;
};

inline const EnemyStats K_ENEMY[] = {
    {20, 30.0f},
    {40, 20.0f},
};

struct SpawnEvent {
  const float time;
  const EnemyType type;
};

// clang-format off
inline const SpawnEvent KLEVEL1[] = {
    {1, BASIC}, {1, BASIC}, {1, BASIC}, {1, BASIC},
    {5, BASIC}, {5, BASIC}, {5, BASIC}, {5, BASIC}, {5, BASIC},
    {8, BASIC}, {8, BASIC},
    {9, BASIC}, {9, BASIC},
    {11, BASIC}, {11, BASIC},
};
// clang-format on

struct Stage {
  const int enemyCount;
  const SpawnEvent *spawns;
};

inline const Stage KSTAGES[] = {
    {sizeof(KLEVEL1) / sizeof(KLEVEL1[0]), KLEVEL1},
};
