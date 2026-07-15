# CLAUDE.md

本文件说明 MyRpg 的当前实现，修改代码时以源码和本文件为准。

## 工作原则

- 先阅读相关代码；只做可验证且与需求直接相关的修改。
- 每个改动批次后使用 x64 Debug 构建验证。
- 保持项目扁平结构，不为单次用途引入复杂抽象。
- 新代码产生未使用字段、函数或包含时应立即清理。

## 构建

在 Visual Studio Developer PowerShell 中运行：

```powershell
msbuild .\MyRpg\MyRpg.vcxproj /p:Configuration=Debug /p:Platform=x64
.\MyRpg\x64\Debug\MyRpg.exe
```

要求：Visual Studio 2022、MSVC v145、C++20、EasyX。项目使用 `winmm.lib`，资源相对可执行文件路径加载。

## 编码风格

- 使用 `#ifndef __CLASS_NAME_H__` include guard，不使用 `#pragma once`。
- Tabs 缩进；使用中文 `//` 注释。
- 类名 PascalCase，方法 camelCase，成员变量无前后缀；常量在 `config.h` 中使用 `constexpr inline`。
- 不使用 `using namespace std`；保持 `std::` 前缀。
- 单例使用函数内 `static` 实例，删除拷贝构造和赋值。
- 默认用构造函数体初始化成员；不要为本项目强制引入不必要的设计模式。

## 架构

```text
rpgMain.cpp
  -> Game::getInstance().run()
  -> SceneManager::run()
  -> StartScene / GameScene / EndScene
```

- `Game`：单例入口。`BatchDrawGuard` 管理 `BeginBatchDraw()` 与 `EndBatchDraw()`；析构时关闭音频、字体和 EasyX 窗口。
- `SceneManager`：创建和切换场景。`TimerResolutionGuard` 管理 `timeBeginPeriod(1)`；主循环以 `steady_clock + sleep_until + yield` 控制 60 FPS。
- `Character`：玩家与敌人的抽象基类，提供移动、血量、攻击、朝向和动画接口。
- `Player`：读取 `Message`，手动移动或左键 A* 点击移动。
- `Enemy`：`updateState()` 只切换动画，移动交给 `FishAI::navigate()`。
- `GameScene`：生成地图，维护玩家、敌人、导航场、战斗、Y 轴排序和 UI。
- `Camera`：单例。`SetViewportOrgEx` 完成世界坐标到屏幕坐标转换；UI 在恢复 viewport 后绘制。
- `AssetManager`：单例资源池。启动时载入 `Frame`，角色动画复用 `Frame*`，不重复解码资源。
- `Message`：单例输入包装，用 `peekmessage` 在一帧内取完消息。

## 导航与碰撞

- 障碍是可重叠的随机矩形；圆形角色碰撞使用最近点法。渲染按矩形并集绘制外轮廓，不使用外接矩形伪合并。
- 地图生成后构建 `40 x 40` 可走网格。玩家出生格 BFS 必须能到达每条地图边；不满足时撤销最后生成的墙块。敌人只从 `reachableGrid` 的边缘格出生。
- 玩家点击时运行 A*：八方向代价 10/14，禁止斜向穿角，终点阻挡时寻找附近可走格，并用 Bresenham 检查平滑路径。
- 敌人直线可见玩家时直接追击。受墙阻挡时使用共享 `navigationField`：玩家跨格才做一次 BFS，敌人沿导航值下降的格子移动。若临近没有下降格，在两格范围内寻找视线可达的下降格；仍失败则停止。
- 敌人局部分离只处理近距离拥挤，不是完整 AFSA 或 ORCA。相关常量为 `FISH_VISUAL_RANGE`、`FISH_SEPARATION_RANGE`、`FISH_STEER_DISTANCE`。

## 关键常量

| 常量 | 值 | 说明 |
| --- | --- | --- |
| `WINDOWS_W/H` | 750 | 窗口尺寸 |
| `WORLD_W/H` | 2000 | 世界尺寸 |
| `ZOOM_RATE` | 5 | 像素资源缩放倍数 |
| `CELL_SIZE` | 50 | 导航网格边长 |
| `FPS` | 60 | 目标帧率 |
| `NAV_CLEARANCE` | 15 | 网格与射线的障碍安全距离 |
| `PATH_WAYPOINT_DISTANCE` | 30 | 玩家切换路径路点距离 |
| `FISH_VISUAL_RANGE` | 70 | 敌人同伴感知距离 |
| `FISH_SEPARATION_RANGE` | 40 | 敌人开始分离距离 |
| `MAX_ENEMIES` | 10 | 最大敌人数量 |

## 文件表

| 文件 | 作用 |
| --- | --- |
| `rpgMain.cpp` | 程序入口 |
| `game.*` | 窗口、资源、批量绘制生命周期 |
| `sceneManager.*`, `scene.h`, `gameState.h` | 场景状态机和帧率控制 |
| `startScene.*`, `gameScene.*`, `endScene.*` | 三个游戏场景 |
| `character.*`, `player.*`, `enemy.*`, `ai.*` | 角色、输入、敌人导航和分离 |
| `collision.*` | 障碍生成、碰撞、A*、导航场 |
| `camera.*`, `message.*` | 相机与输入单例 |
| `asset.*`, `frame.*`, `animation.*` | 共享帧资源和动画播放 |
| `alpha.*`, `edit.*`, `utils.*` | 图像处理与辅助函数 |
| `config.h` | 全局编译期常量 |
| `source/` | 图片、字体和音频 |

## 已知限制

- 敌人只有局部分离，不做角色间刚体碰撞或完整 ORCA。
- 墙体运行时改变后，必须重新建立可走网格、可达网格和导航场。
- 随从系统暂缓，计划位于 `TODO.md`。
