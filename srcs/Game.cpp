#include "Game.hpp"
#include "Gameplay.hpp"
#include "raylib.h"

Game::Game(int width, int height, const std::string &title)
    : m_width(width), m_height(height) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);

  SetExitKey(KEY_NULL); // ESC no longer sets WindowShouldClose
}

Game::~Game() { CloseWindow(); }

void Game::update() {
  // NOTE: was going to make pressing X on window show confirmation
  // but was causing bugs in WSL so only pressing ESC shows confirmation
  if (WindowShouldClose()) {
    m_exitWindow = true;
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
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

    if (IsKeyPressed(KEY_ENTER)) {
      m_currentScreen = GAMEPLAY;
      m_gameplay.reset();
    }
  } break;
  case GAMEPLAY: {
    m_gameState = m_gameplay.update();
    if (m_gameState != PLAYING) {
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

void Game::draw() {
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
    m_gameplay.draw();
  } break;
  case ENDING: {
    if (m_gameState == WON) {
      DrawText("CUSTOMERS FED", 10, 10, 30, BLACK);
    } else if (m_gameState == DIED) {
      DrawText("YOU LOST", 10, 10, 30, RED);
    }
    if ((m_frameCounter / 30) % 2 == 0) {
      DrawText("Press [ENTER] to go back to title",
               GetScreenWidth() / 2 -
                   MeasureText("Press [Enter] to go back to title", 20) / 2,
               GetScreenHeight() / 2 + 60, 20, DARKGRAY);
    }
  } break;
  default:
    break;
  }

  if (m_exitWindowRequested) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(DARKGRAY, 0.5));
    DrawRectangle(0, 100, m_width, 200, BLACK);
    DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30, WHITE);
  }

  EndDrawing();
}
