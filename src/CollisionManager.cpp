#include "CollisionManager.h"

#include <algorithm>
#include <cmath>

void CollisionManager::handleWallCollision(Ball& ball, int screenWidth,
                                           int screenHeight) {
  const Vector2 pos = ball.getPosition();
  const Vector2 vel = ball.getVelocity();
  const float radius = ball.getRadius();

  // 左右边界反弹
  if (pos.x - radius <= 0.0f && vel.x < 0.0f) {
    ball.reverseX();
  } else if (pos.x + radius >= static_cast<float>(screenWidth) && vel.x > 0.0f) {
    ball.reverseX();
  }

  // 上边界反弹
  if (pos.y - radius <= 0.0f && vel.y < 0.0f) {
    ball.reverseY();
  }
}

void CollisionManager::handlePaddleCollision(Ball& ball, const Paddle& paddle) {
  const Vector2 pos = ball.getPosition();
  const float radius = ball.getRadius();

  if (checkCircleRect(pos, radius, paddle.getRect())) {
    // 仅当小球向下运动时反弹，避免连续反转
    if (ball.getVelocity().y > 0.0f) {
      ball.reverseY();
    }
  }
}

void CollisionManager::handleBrickCollisions(Ball& ball,
                                             std::vector<Brick>& bricks,
                                             int& score) {
  const Vector2 pos = ball.getPosition();
  const float radius = ball.getRadius();

  for (auto& brick : bricks) {
    if (!brick.isActive()) {
      continue;
    }

    if (checkCircleRect(pos, radius, brick.getRect())) {
      brick.hit();
      score += 1;
      ball.reverseY();
      break;
    }
  }
}

bool CollisionManager::checkCircleRect(const Vector2& center, float radius,
                                       const Rectangle& rect) {
  // 纯数学判定，避免依赖图形库的碰撞函数，便于单元测试
  const float closestX = std::clamp(center.x, rect.x, rect.x + rect.width);
  const float closestY = std::clamp(center.y, rect.y, rect.y + rect.height);

  const float dx = center.x - closestX;
  const float dy = center.y - closestY;
  return (dx * dx + dy * dy) <= (radius * radius);
}
