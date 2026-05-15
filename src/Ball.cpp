#include "Ball.h"

void Ball::update(float deltaTime) {
  // 仅处理位置更新，便于单元测试
  position_.x += velocity_.x * deltaTime;
  position_.y += velocity_.y * deltaTime;
}

void Ball::draw() const {
  DrawCircleV(position_, radius_, BLUE);
}

void Ball::reverseX() {
  velocity_.x = -velocity_.x;
}

void Ball::reverseY() {
  velocity_.y = -velocity_.y;
}

void Ball::reset(const Vector2& position, const Vector2& velocity, float radius) {
  position_ = position;
  velocity_ = velocity;
  radius_ = radius;
}

Vector2 Ball::getPosition() const {
  return position_;
}

Vector2 Ball::getVelocity() const {
  return velocity_;
}

float Ball::getRadius() const {
  return radius_;
}

bool Ball::isOutOfBounds(float screenHeight) const {
  // 只判断小球是否完全越过屏幕下方
  return position_.y - radius_ > screenHeight;
}
