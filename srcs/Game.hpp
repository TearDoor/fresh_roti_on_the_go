#pragma once

#include "Gameplay.hpp"

#include <string>

enum GameScreen { TITLE = 0, GAMEPLAY, ENDING };

class Game {
public:
  Game(int width, int height, const std::string &title);
  ~Game();

  void update();
  void draw();

  bool shouldClose() const { return m_exitWindow; }

private:
  int m_width;
  int m_height;
  GameScreen m_currentScreen = TITLE;
  bool m_exitWindowRequested = false;
  bool m_exitWindow = false;
  int m_frameCounter = 0;

  Gameplay m_gameplay;
  GameState m_gameState;

  Assets m_assets;
};
