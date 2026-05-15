#include <iostream>
#include <vector>

#include "../src/Ball.h"
#include "../src/Brick.h"
#include "../src/CollisionManager.h"
#include "../src/Paddle.h"

namespace {
constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;

int g_failed = 0;

void expect(const char* name, bool condition) {
  if (condition) {
    std::cout << "[PASS] " << name << "\n";
  } else {
    std::cout << "[FAIL] " << name << "\n";
    ++g_failed;
  }
}

void test_ball_wall_bounce() {
  Ball ball;

  ball.reset({5.0f, 100.0f}, {-100.0f, 0.0f}, 5.0f);
  CollisionManager::handleWallCollision(ball, kScreenWidth, kScreenHeight);
  expect("Ball bounces on left wall", ball.getVelocity().x > 0.0f);

  ball.reset({kScreenWidth - 5.0f, 120.0f}, {100.0f, 0.0f}, 5.0f);
  CollisionManager::handleWallCollision(ball, kScreenWidth, kScreenHeight);
  expect("Ball bounces on right wall", ball.getVelocity().x < 0.0f);

  ball.reset({200.0f, 4.0f}, {0.0f, -80.0f}, 5.0f);
  CollisionManager::handleWallCollision(ball, kScreenWidth, kScreenHeight);
  expect("Ball bounces on top wall", ball.getVelocity().y > 0.0f);
}

void test_ball_paddle_collision() {
  Ball ball;
  Paddle paddle;

  paddle.reset({300.0f, 500.0f}, {120.0f, 20.0f}, 300.0f);
  ball.reset({360.0f, 510.0f}, {0.0f, 100.0f}, 6.0f);

  CollisionManager::handlePaddleCollision(ball, paddle);
  expect("Ball bounces on paddle", ball.getVelocity().y < 0.0f);
}

void test_ball_brick_collision_and_score() {
  Ball ball;
  std::vector<Brick> bricks;
  int score = 0;

  bricks.emplace_back(Vector2{100.0f, 100.0f}, Vector2{60.0f, 20.0f});
  ball.reset({110.0f, 110.0f}, {0.0f, 120.0f}, 5.0f);

  CollisionManager::handleBrickCollisions(ball, bricks, score);

  expect("Brick becomes inactive after hit", !bricks[0].isActive());
  expect("Score increases after hit", score == 1);
}
}  // namespace

int main() {
  std::cout << "=== Logic Tests (CppBallGame) ===\n";

  test_ball_wall_bounce();
  test_ball_paddle_collision();
  test_ball_brick_collision_and_score();

  if (g_failed == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  }

  std::cout << g_failed << " test(s) failed.\n";
  return 1;
}
