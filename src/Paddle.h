#pragma once

#include "raylib.h"

// 挡板类：负责玩家控制与绘制
class Paddle {
 public:
  Paddle() = default;

  // 初始化挡板状态
  void reset(const Vector2& position, const Vector2& size, float speed);
  // 处理键盘输入
  void handleInput();
  // 更新挡板位置
  void update(float deltaTime);
  // 绘制挡板
  void draw() const;

  // 获取用于碰撞检测的矩形
  Rectangle getRect() const;

 private:
  Vector2 position_{0.0f, 0.0f};
  Vector2 size_{0.0f, 0.0f};
  float speed_ = 0.0f;
  float moveDir_ = 0.0f;
};
