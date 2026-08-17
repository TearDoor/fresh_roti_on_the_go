#include "raylib.h"

int main(void) {
  const int WIDTH = 800;
  const int HEIGHT = 600;

  // TODO: rename window
  InitWindow(WIDTH, HEIGHT, "untitled Malaysian game");

  bool exitWindowRequested = false;
  bool exitWindow = false;

  SetTargetFPS(60);

  while (!exitWindow) {
    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
      exitWindowRequested = true;
    }

    if (exitWindowRequested) {
      if (IsKeyPressed(KEY_Y))
        exitWindow = true;
      else if (IsKeyPressed(KEY_N)) {
        exitWindowRequested = false;
      }
    }

    BeginDrawing();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
