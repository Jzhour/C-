#include "Game.h"

#include "CollisionManager.h"
#include "raylib.h"

namespace {
constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;
constexpr int kTargetFps = 60;

constexpr float kBallRadius = 8.0f;
const Vector2 kBallStartPos = {kScreenWidth / 2.0f, kScreenHeight / 2.0f};
const Vector2 kBallStartVel = {200.0f, -200.0f};

constexpr float kPaddleWidth = 100.0f;
constexpr float kPaddleHeight = 20.0f;
const Vector2 kPaddleStartPos = {kScreenWidth / 2.0f, kScreenHeight - 40.0f};
constexpr float kPaddleSpeed = 400.0f;

constexpr int kBrickRows = 5;
constexpr int kBrickCols = 10;
constexpr float kBrickWidth = 60.0f;
constexpr float kBrickHeight = 20.0f;
constexpr float kBrickSpacing = 10.0f;
constexpr float kBrickStartX = 60.0f;
constexpr float kBrickStartY = 60.0f;
}  // namespace

Game::Game() {
  // 初始化窗口与游戏对象
  init();
}

Game::~Game() = default;

void Game::run() {
  // 标准游戏循环
  while (!WindowShouldClose()) {
    const float deltaTime = GetFrameTime();

    handleInput();
    update(deltaTime);
    render();
  }

  CloseWindow();
}

void Game::init() {
  // 创建窗口并设置帧率
  InitWindow(kScreenWidth, kScreenHeight, "CppBallGame");
  SetTargetFPS(kTargetFps);

  resetGame();
  state_ = GameState::Start;
}

void Game::resetGame() {
  // 初始化游戏状态
  score_ = 0;
  currentLevel_ = 1;

  // 初始化砖块阵列
  loadLevel(currentLevel_);
}

void Game::resetActors() {
  ball_.reset(kBallStartPos, kBallStartVel, kBallRadius);
  paddle_.reset(kPaddleStartPos, {kPaddleWidth, kPaddleHeight}, kPaddleSpeed);
}

void Game::loadLevel(int level) {
  // 根据关卡编号初始化砖块阵列
  bricks_.clear();
  bricks_.reserve(kBrickRows * kBrickCols);

  if (level == 1) {
    for (int row = 0; row < kBrickRows; ++row) {
      for (int col = 0; col < kBrickCols; ++col) {
        const Vector2 pos = {
            kBrickStartX + col * (kBrickWidth + kBrickSpacing),
            kBrickStartY + row * (kBrickHeight + kBrickSpacing)};
        bricks_.emplace_back(pos, Vector2{kBrickWidth, kBrickHeight});
      }
    }

    // 加载关卡后重置小球与挡板位置
    resetActors();
    return;
  }

  // 第二关：金字塔排列，形状与第一关完全不同
  for (int row = 0; row < kBrickRows; ++row) {
    const int rowCount = 1 + row * 2;
    const float rowWidth =
        rowCount * kBrickWidth + (rowCount - 1) * kBrickSpacing;
    const float startX = (kScreenWidth - rowWidth) / 2.0f;

    for (int col = 0; col < rowCount; ++col) {
      const Vector2 pos = {
          startX + col * (kBrickWidth + kBrickSpacing),
          kBrickStartY + row * (kBrickHeight + kBrickSpacing)};
      bricks_.emplace_back(pos, Vector2{kBrickWidth, kBrickHeight});
    }
  }

  // 加载关卡后重置小球与挡板位置
  resetActors();
}

void Game::handleInput() {
  // Start/Win/GameOver 状态下按 Enter 进入或重新开始
  if (state_ == GameState::Start) {
    if (IsKeyPressed(KEY_ENTER)) {
      resetGame();
      state_ = GameState::Playing;
    }
    return;
  }

  if (state_ == GameState::Win) {
    if (IsKeyPressed(KEY_ENTER)) {
      resetGame();
      state_ = GameState::Start;
    }
    return;
  }

  if (state_ == GameState::GameOver) {
    if (currentLevel_ == 1) {
      if (IsKeyPressed(KEY_ENTER)) {
        currentLevel_ = 2;
        score_ = 0;
        loadLevel(2);
        state_ = GameState::Playing;
      } else if (IsKeyPressed(KEY_ESCAPE)) {
        resetGame();
        state_ = GameState::Start;
      }
    } else {
      if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        resetGame();
        state_ = GameState::Start;
      }
    }
    return;
  }

  // 仅在游戏进行中响应输入
  if (state_ == GameState::Playing) {
    paddle_.handleInput();
  }
}

void Game::update(float deltaTime) {
  // 更新游戏状态（仅在进行中更新）
  if (state_ != GameState::Playing) {
    return;
  }

  ball_.update(deltaTime);
  paddle_.update(deltaTime);

  // 碰撞检测与处理
  CollisionManager::handleWallCollision(ball_, kScreenWidth, kScreenHeight);
  CollisionManager::handlePaddleCollision(ball_, paddle_);
  CollisionManager::handleBrickCollisions(ball_, bricks_, score_);

  checkWinLose();
}

void Game::render() const {
  // 绘制界面
  BeginDrawing();
  ClearBackground(RAYWHITE);

  if (state_ == GameState::Start) {
    const char* title = "BALL BOUNCE GAME";
    const char* line1 = "LEFT/RIGHT: Move Paddle";
    const char* line2 = "Clear all bricks to advance";
    const char* line3 = "Miss the ball to lose";
    const char* line4 = "Press ENTER to Start";

    const int titleSize = 48;
    const int textSize = 22;
    const int lineGap = 8;

    const int titleWidth = MeasureText(title, titleSize);
    DrawText(title, (kScreenWidth - titleWidth) / 2, 160, titleSize, DARKBLUE);
    DrawLine(180, 225, kScreenWidth - 180, 225, LIGHTGRAY);

    const int startY = 250;
    const int lineHeight = textSize + lineGap;
    const int line1Width = MeasureText(line1, textSize);
    const int line2Width = MeasureText(line2, textSize);
    const int line3Width = MeasureText(line3, textSize);
    const int line4Width = MeasureText(line4, textSize);

    DrawText(line1, (kScreenWidth - line1Width) / 2, startY, textSize, DARKGRAY);
    DrawText(line2, (kScreenWidth - line2Width) / 2, startY + lineHeight,
             textSize, DARKGRAY);
    DrawText(line3, (kScreenWidth - line3Width) / 2, startY + lineHeight * 2,
             textSize, DARKGRAY);
    DrawText(line4, (kScreenWidth - line4Width) / 2, startY + lineHeight * 3,
             textSize, DARKGRAY);
    EndDrawing();
    return;
  }

  ball_.draw();
  paddle_.draw();
  for (const auto& brick : bricks_) {
    if (brick.isActive()) {
      brick.draw();
    }
  }

  DrawText(TextFormat("Score: %i  Level: %i", score_, currentLevel_), 10, 10, 20,
           DARKGRAY);

  if (state_ == GameState::Win) {
    const char* msg = "YOU WIN!";
    const int textWidth = MeasureText(msg, 40);
    DrawText(msg, (kScreenWidth - textWidth) / 2, kScreenHeight / 2 - 20, 40,
             GREEN);
    const char* hint = "Press ENTER to Return";
    const int hintWidth = MeasureText(hint, 20);
    DrawText(hint, (kScreenWidth - hintWidth) / 2, kScreenHeight / 2 + 30, 20,
             DARKGRAY);
  } else if (state_ == GameState::GameOver) {
    if (currentLevel_ == 1) {
      const char* msg = "LEVEL 1 FAILED";
      const char* hint1 = "Press ENTER to Play Level 2";
      const char* hint2 = "Press ESC to Return";
      const int textWidth = MeasureText(msg, 36);
      const int hint1Width = MeasureText(hint1, 20);
      const int hint2Width = MeasureText(hint2, 20);
      DrawText(msg, (kScreenWidth - textWidth) / 2, kScreenHeight / 2 - 40, 36,
               RED);
      DrawText(hint1, (kScreenWidth - hint1Width) / 2, kScreenHeight / 2 + 10,
               20, DARKGRAY);
      DrawText(hint2, (kScreenWidth - hint2Width) / 2, kScreenHeight / 2 + 40,
               20, DARKGRAY);
    } else {
      const char* msg = "GAME OVER";
      const int textWidth = MeasureText(msg, 40);
      DrawText(msg, (kScreenWidth - textWidth) / 2, kScreenHeight / 2 - 20, 40,
               RED);
      const char* hint = "Press ENTER to Return";
      const int hintWidth = MeasureText(hint, 20);
      DrawText(hint, (kScreenWidth - hintWidth) / 2, kScreenHeight / 2 + 30, 20,
               DARKGRAY);
    }
  }

  EndDrawing();
}

void Game::checkWinLose() {
  // 判断失败：小球掉出屏幕下方
  if (ball_.isOutOfBounds(static_cast<float>(kScreenHeight))) {
    state_ = GameState::GameOver;
    return;
  }

  // 判断胜利：所有砖块被清除
  bool allCleared = true;
  for (const auto& brick : bricks_) {
    if (brick.isActive()) {
      allCleared = false;
      break;
    }
  }

  if (!bricks_.empty() && allCleared) {
    if (currentLevel_ == 1) {
      currentLevel_ = 2;
      loadLevel(2);
      state_ = GameState::Playing;
    } else if (currentLevel_ == 2) {
      state_ = GameState::Win;
    }
  }
}
