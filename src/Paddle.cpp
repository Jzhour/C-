#include "Paddle.h"

void Paddle::reset(const Vector2& position, const Vector2& size, float speed) {
  position_ = position;
  size_ = size;
  speed_ = speed;
  moveDir_ = 0.0f;
}

void Paddle::handleInput() {
  // 读取左右键输入，得到移动方向
  moveDir_ = 0.0f;
  if (IsKeyDown(KEY_LEFT)) {
    moveDir_ -= 1.0f;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    moveDir_ += 1.0f;
  }
}

void Paddle::update(float deltaTime) {
  // 根据方向与速度更新位置
  position_.x += moveDir_ * speed_ * deltaTime;

  // 防止挡板移出窗口边界
  const int screenWidth = GetScreenWidth();
  if (position_.x < 0.0f) {
    position_.x = 0.0f;
  }
  if (position_.x + size_.x > static_cast<float>(screenWidth)) {
    position_.x = static_cast<float>(screenWidth) - size_.x;
  }
}

void Paddle::draw() const {
  DrawRectangleV(position_, size_, DARKBLUE);
}

Rectangle Paddle::getRect() const {
  return Rectangle{position_.x, position_.y, size_.x, size_.y};
}
