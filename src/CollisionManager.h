#pragma once

#include <vector>

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

// 碰撞管理类：集中处理小球与场景对象的碰撞
class CollisionManager {
 public:
  // 小球与窗口边界碰撞
  static void handleWallCollision(Ball& ball, int screenWidth, int screenHeight);
  // 小球与挡板碰撞
  static void handlePaddleCollision(Ball& ball, const Paddle& paddle);
  // 小球与砖块碰撞
  static void handleBrickCollisions(Ball& ball, std::vector<Brick>& bricks,
                                    int& score);

 private:
  // 圆形与矩形碰撞检测
  static bool checkCircleRect(const Vector2& center, float radius,
                              const Rectangle& rect);
};
