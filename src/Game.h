#pragma once

#include <vector>

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

// 游戏主流程管理类
class Game {
 public:
  // 构造与析构
  Game();
  ~Game();

  // 游戏主循环入口
  void run();

 private:
  // 初始化窗口与资源
  void init();
  // 重置游戏对象与分数
  void resetGame();
  // 重置小球与挡板位置
  void resetActors();
  // 根据关卡编号初始化砖块布局
  void loadLevel(int level);
  // 处理输入
  void handleInput();
  // 更新游戏状态
  void update(float deltaTime);
  // 绘制游戏画面
  void render() const;
  // 判断胜负状态
  void checkWinLose();

 private:
  Ball ball_;
  Paddle paddle_;
  std::vector<Brick> bricks_;

  int score_ = 0;
  int currentLevel_ = 1;
  int maxLevel_ = 2;

  enum class GameState {
    Start,
    Playing,
    Win,
    GameOver
  };

  GameState state_ = GameState::Start;
};
