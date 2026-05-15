#pragma once

#include "raylib.h"

// 砖块类：负责状态与绘制
class Brick {
 public:
  Brick() = default;
  Brick(const Vector2& position, const Vector2& size);

  // 绘制砖块
  void draw() const;
  // 砖块被击中
  void hit();

  // 砖块是否仍然可见
  bool isActive() const;
  // 获取用于碰撞检测的矩形
  Rectangle getRect() const;

 private:
  Rectangle rect_{0.0f, 0.0f, 0.0f, 0.0f};
  bool active_ = true;
};
