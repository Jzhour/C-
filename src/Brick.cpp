#include "Brick.h"

Brick::Brick(const Vector2& position, const Vector2& size)
    : rect_{position.x, position.y, size.x, size.y}, active_(true) {}

void Brick::draw() const {
  if (!active_) {
    return;
  }

  DrawRectangleRec(rect_, ORANGE);
  DrawRectangleLinesEx(rect_, 1.0f, BROWN);
}

void Brick::hit() {
  // 被击中后变为不可见
  active_ = false;
}

bool Brick::isActive() const {
  return active_;
}

Rectangle Brick::getRect() const {
  return rect_;
}
