#include "Game.hpp"
#include "raylib.h"

Game::Game(int width, int height, const std::string &title)
    : m_width(width), m_height(height) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);
}

Game::~Game() { CloseWindow(); }

void Game::Update() {
  if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
    m_exitWindowRequested = true;
  }

  if (m_exitWindowRequested) {
    if (IsKeyPressed(KEY_Y))
      m_exitWindow = true;
    else if (IsKeyPressed(KEY_N)) {
      m_exitWindowRequested = false;
    }
    return;
  }

  switch (m_currentScreen) {
  case TITLE: {
    m_frameCounter++;

    if (IsKeyPressed(KEY_ENTER))
      m_currentScreen = GAMEPLAY;
  } break;
  case GAMEPLAY: {
    m_gameplay.Update();

    if (IsKeyPressed(KEY_ENTER)) {
      m_currentScreen = ENDING;
    }
  } break;
  case ENDING: {
    m_frameCounter++;
    if (IsKeyPressed(KEY_ENTER)) {
      m_currentScreen = TITLE;
      m_frameCounter = 0;
    }
  } break;
  default:
    break;
  }
}

void Game::Draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  switch (m_currentScreen) {
  case TITLE: {
    if ((m_frameCounter / 30) % 2 == 0) {
      DrawText("Press [ENTER] to start",
               GetScreenWidth() / 2 -
                   MeasureText("Press [Enter] to start", 20) / 2,
               GetScreenHeight() / 2 + 60, 20, DARKGRAY);
    }
  } break;
  case GAMEPLAY: {
    m_gameplay.Draw();
  } break;
  default:
    break;
  }

  if (m_exitWindowRequested) {
    DrawRectangle(0, 100, m_width, 200, BLACK);
    DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30, WHITE);
  }

  EndDrawing();
}
