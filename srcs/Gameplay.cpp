#include "Gameplay.hpp"

void Gameplay::Update() { return; }

void Gameplay::Draw() {
  // draw player
  Vector2 tip;
  Vector2 backLeft;
  Vector2 backRight;

  tip = {m_player.m_position.x, m_player.m_position.y - 20.0f};
  backLeft = {m_player.m_position.x - 10.0f, m_player.m_position.y + 20.0f};
  backRight = {m_player.m_position.x + 10.0f, m_player.m_position.y + 20.0f};

  DrawTriangle(tip, backLeft, backRight, BLUE);
}
