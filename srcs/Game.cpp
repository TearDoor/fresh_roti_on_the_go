#include "Game.hpp"
#include "Content.hpp"
#include "Gameplay.hpp"
#include "raylib.h"

Game::Game(int width, int height, const std::string &title)
    : m_width(width), m_height(height) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);

  SetExitKey(KEY_NULL); // ESC no longer sets WindowShouldClose

  m_assets.tilemap = LoadTexture("resources/tilemap/tilemap.png");
  m_assets.texRoti = LoadTexture("resources/textures/roti_canai.png");
  m_assets.texFlag = LoadTexture("resources/textures/Malaysia_flag.png");
  m_assets.texPlayer = LoadTexture("resources/textures/player.png");

  InitAudioDevice();

  m_assets.fxShoot = LoadSound("resources/sounds/shoot.wav");
  m_assets.fxHurt = LoadSound("resources/sounds/hurt.wav");
  m_assets.fxMotor = LoadSound("resources/sounds/motorcycle.mp3");
  m_assets.fxWin = LoadSound("resources/sounds/malaysia.wav");

  m_assets.fontTitle = LoadFont("resources/font/NotJamLaika14.ttf");
}

Game::~Game() {
  UnloadFont(m_assets.fontTitle);

  UnloadSound(m_assets.fxShoot);
  UnloadSound(m_assets.fxHurt);
  UnloadSound(m_assets.fxMotor);
  UnloadSound(m_assets.fxWin);

  CloseAudioDevice();

  UnloadTexture(m_assets.texRoti);
  UnloadTexture(m_assets.tilemap);
  UnloadTexture(m_assets.texFlag);
  UnloadTexture(m_assets.texPlayer);

  CloseWindow();
}

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
    m_gameState = m_gameplay.update(m_assets);
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

  // DrawTexturePro(
  //     m_assets.texFlag,
  //     {0, 0, (float)m_assets.texFlag.width, (float)m_assets.texFlag.height},
  //     {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0},
  //     0.0f, Fade(WHITE, 0.5f));
  switch (m_currentScreen) {
  case TITLE: {
    ClearBackground(BEIGE);
    DrawTextEx(m_assets.fontTitle, "FRESH ROTI\nON THE GO", {100.0f, 100.0f},
               100, 0.0f, BROWN);
    if ((m_frameCounter / 30) % 2 == 0) {
      DrawText("Press [ENTER] to start",
               GetScreenWidth() / 2 -
                   MeasureText("Press [Enter] to start", 20) / 2,
               GetScreenHeight() / 2 + 60, 20, RAYWHITE);
    }
  } break;
  case GAMEPLAY: {
    // draw background
    for (int y = 0; y < MAP_ROWS; y++) {
      for (int x = 0; x < MAP_COLS; x++) {
        int index = x + (y * MAP_COLS);

        Rectangle src = m_assets.getRectFromID(KTILEMAP[index]);
        Rectangle dst = {(float)x * MAP_TILESIZE, (float)y * MAP_TILESIZE, 32,
                         32};

        DrawTexturePro(m_assets.tilemap, src, dst, {0, 0}, 0.0f, WHITE);
      }
    }

    m_gameplay.draw(m_assets);
  } break;
  case ENDING: {
    if (m_gameState == WON) {
      ClearBackground(BEIGE);
      DrawTextEx(m_assets.fontTitle, "CUSTOMERS FED", {100.0f, 100.0f}, 100,
                 0.0f, BLACK);
    } else if (m_gameState == DIED) {
      ClearBackground(DARKGRAY);
      DrawText("YOU LOST", 100, 100, 30, RED);
    }
    if ((m_frameCounter / 30) % 2 == 0) {
      DrawText("Press [ENTER] to go back to title",
               GetScreenWidth() / 2 -
                   MeasureText("Press [Enter] to go back to title", 20) / 2,
               GetScreenHeight() / 2 + 60, 20, RAYWHITE);
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
