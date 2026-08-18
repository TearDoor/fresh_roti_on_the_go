#include "Game.hpp"

int main(void) {
  const int WIDTH = 800;
  const int HEIGHT = 600;

  // TODO: rename window
  Game game(WIDTH, HEIGHT, "Untitled Malaysian game");

  while (!game.shouldClose()) {
    game.update();
    game.draw();
  }

  return 0;
}
