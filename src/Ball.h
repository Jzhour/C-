#pragma once

#include "raylib.h"

// 小球类：负责位置、速度、半径以及运动与绘制
class Ball {
 public:
  Ball() = default;

  // 更新小球位置
  void update(float deltaTime);
  // 绘制小球
  void draw() const;

  // 速度在 X/Y 方向反转
  void reverseX();
  void reverseY();

  // 重置小球状态
  void reset(const Vector2& position, const Vector2& velocity, float radius);

  // 基础属性访问
  Vector2 getPosition() const;
  Vector2 getVelocity() const;
  float getRadius() const;

  // 判断小球是否越过屏幕下方
  bool isOutOfBounds(float screenHeight) const;

 private:
  Vector2 position_{0.0f, 0.0f};
  Vector2 velocity_{0.0f, 0.0f};
  float radius_ = 0.0f;
};
