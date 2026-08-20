#include "Content.hpp"

Rectangle Assets::getRectFromID(int id) {
  int index = id - 1;
  int x = (index % TILEMAP_COLS) * (TILEMAP_TILESIZE + 1);
  int y = (index / TILEMAP_COLS) * (TILEMAP_TILESIZE + 1);

  return (Rectangle){(float)x, (float)y, TILEMAP_TILESIZE, TILEMAP_TILESIZE};
}
