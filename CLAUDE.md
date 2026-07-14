# CLAUDE.md

This file provides guidance to Claude Code when working with code in this repository.

## Behavioral Rules

These apply to every task. They bias toward caution and simplicity.

### 1. Think Before Coding

- State assumptions explicitly. If uncertain, ask — don't guess silently.
- If multiple approaches exist, present tradeoffs — don't pick one without explaining.
- If something is unclear, stop and name what's confusing.

### 2. Simplicity First

- Minimum code that solves the problem. Nothing speculative.
- No abstractions for single-use code.
- If you write 200 lines and it could be 50, rewrite it.
- Ask: "Would the author think this is overcomplicated?" If yes, simplify.

### 3. Surgical Changes

- Touch only what you must. Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- When your changes create orphans (unused imports, variables), clean up only those.
- Don't remove pre-existing dead code unless asked.
- Every changed line should trace directly to the user's request.

### 4. Goal-Driven Execution

- Transform tasks into verifiable goals: "Write test → make it pass" rather than "fix it."
- For multi-step tasks, state a brief plan with verification checkpoints.
- **Build after every batch of changes.** Don't write 10 files then compile — incremental fixes prevent cascading errors.

---

## Build

```bash
msbuild MyRpg.sln /p:Configuration=Debug /p:Platform=x64
```
- VS 2022+ required. Toolset: v145. C++20, `/utf-8` flag.
- Exe needs `source/` folder alongside (post-build xcopy auto-copies assets).
- No test framework or linter configured.

---

## Author's Coding Style

These conventions are derived from the author's full project history (MyRpg, snake, easyx2-6, cpp_learn). Follow them exactly — do not impose generic C++ guidelines over the author's preferences.

### Include Guards

```cpp
#ifndef __CLASS_NAME_H__
#define __CLASS_NAME_H__
// ...
#endif
```
Always double-underscore `__UPPERCASE_H__`. Never `#pragma once`.

### Naming

| Element | Convention | Examples |
|---------|-----------|----------|
| Classes | PascalCase | `Player`, `GameScene`, `SceneManager` |
| Methods | camelCase | `updateState()`, `getX()`, `onFrame()` |
| Members | bare, no prefix | `score`, `enemyList`, `currentScene` |
| Locals | camelCase or short | `delta_x`, `i`, `msg`, `path` |
| Constants | UPPER_SNAKE | `WINDOWS_W`, `MAX_ENEMIES` |
| Enum values | PascalCase | `START`, `PLAYING`, `GameOver` |

### Indentation & Comments

- **Tabs** (rendered as 4-space width)
- **Chinese comments**, `//` single-line only. No `/* */` doc blocks.

### Singletons

```cpp
class Foo {
public:
    static Foo &getInstance();          // Returns reference (not pointer)
    Foo(const Foo &) = delete;          // Never copyable
    Foo &operator=(const Foo &) = delete;
private:
    Foo();                              // Private constructor
};
// Implementation:
Foo &Foo::getInstance() { static Foo instance; return instance; }
```
Local-static pattern (C++11 thread-safe). Used by `Game`, `Camera`.

### Constructors

Body assignment is the author's default. Initializer lists only for trivial forwarding.
```cpp
Panel::Panel() {
    this->score = 0;
    this->time  = GAME_T;
}
```

### Other Conventions

- **No `using namespace std`** — always `std::` prefix
- **Raw pointers acceptable** — the author uses `new`/`delete`; don't force smart pointers
- **Don't over-engineer** — simple `if`/`switch`/`goto` over heavy design patterns
- **`constexpr` for compile-time constants** in headers (not `#define`)
- **No `m_` prefix, no Hungarian notation, no trailing `_`**

---

## Project Architecture

```
rpg_main.cpp  (3 lines)
  └─ Game::getInstance().run()
       └─ SceneManager::run()
            ├─ StartScene   → Playing
            ├─ GameScene    → GameOver (on death)
            └─ EndScene     → Playing (ENTER) / Quit (ESC)

Singleton controllers: Game (lifecycle), Camera (viewport)
```

### Entry Point
```cpp
// rpg_main.cpp
#include "game.h"
int main() { return Game::getInstance().run(); }
```

### Scene State Machine
- `GameState` enum: `Running | Start | Playing | GameOver | Quit`
- `Scene` base: `onEnter()` → `onFrame(SceneManager&)` → `onExit()`
- `onFrame()` returns next desired `GameState`. `Running` means continue.
- `SceneManager` handles creation, transition sequencing, and score passing.
- `BeginBatchDraw()` once in `Game::run()`, `FlushBatchDraw()` per frame in each scene.

### Character System
```
Character (abstract base, raw Animation* to derived-class arrays)
├── Player  — input via peekmessage, debug toggle (F1)
└── Enemy   — pursuit AI via move2()
```
Player/Enemy each own their `Frame[]` and `Animation[]` arrays. `Character` stores a raw `Animation*` pointer set via `set(x, y, animations, count)`. The author intentionally keeps derived classes as data owners.

### Camera
- Singleton: `Camera::getInstance()`
- `init(worldW, worldH, screenW, screenH)` — once per game
- `setTarget(x, y)` + `update()` — each frame, lerps toward player
- Rendering uses `SetViewportOrgEx(hdc, -offsetX, -offsetY)` for world→screen transform
- UI elements rendered after restoring viewport origin (screen-space, not camera-affected)

### Rendering Pipeline
```
Frame::set() → loadimage → ZoomImage(5x) → FlipImage → SketchImage (hurt flash)
Animation::play() → cycles frames on DELTA_TIME timer, handles flip
Character::updateAnimation() → play current anim → draw HP bar
GameScene render:
  1. cleardevice()
  2. SetViewportOrgEx(camera offset)
  3. Y-sorted renderOrder (Player + Enemies, low Y first)
  4. UI (score, time) — after restoring viewport
  5. FlushBatchDraw()
  6. Sleep to meet 60fps target
```

### Key Constants (config.h)

| Constant | Value | Purpose |
|----------|-------|---------|
| `WINDOWS_W/H` | 750 | Viewport size |
| `WORLD_W/H` | 2000 | Game world (larger than viewport → camera scrolls) |
| `ZOOM_RATE` | 5 | Sprite upscale factor |
| `FPS` | 60 | Target frame rate |
| `DELTA_TIME` | 6 | Animation frame advance interval |
| `CAMERA_LERP` | 0.08 | Camera smooth-follow factor |
| `MAX_ENEMIES` | 10 | Max simultaneous enemies |
| `HP_BONUS_INTERVAL` | 10 | Kills per HP bonus |

---

## Game Mechanics

- **Player**: WASD/Arrows move, J attack, left-click move-to, right-click attack-toward
- **Enemies**: spawn from world edges, pursue player, attack when in range
- **Combat**: circle-based collision (`getAttackX/Y` vs `getCenterX/Y`), invuln frames (1.5× `DELTA_TIME`)
- **Scoring**: +1 per kill, every 10 kills gives +20 HP
- **F1**: toggle debug overlay (collision circles, enemy lines, world boundary)

---

## File Map

| File | Role |
|------|------|
| `rpg_main.cpp` | Entry point — 3 lines |
| `config.h` | All constants (header-only) |
| `game_state.h` | `GameState` enum (header-only) |
| `game.h/cpp` | Singleton — resources, window, lifecycle |
| `scene.h` | Abstract `Scene` interface (header-only) |
| `scene_manager.h/cpp` | State machine, transitions, score pass-through |
| `start_scene.h/cpp` | Title screen + controls help |
| `game_scene.h/cpp` | Full game loop (input→update→combat→render) |
| `end_scene.h/cpp` | Score display, restart/quit |
| `character.h/cpp` | Base — movement, combat, health, animation |
| `player.h/cpp` | Input handling (`getMessage`) |
| `enemy.h/cpp` | AI pursuit |
| `camera.h/cpp` | Singleton — viewport transform, smooth follow |
| `timer.h/cpp` | High-res `QueryPerformanceCounter` timer |
| `message.h` | `VK_*` virtual-key constants (header-only) |
| `frame.h/cpp` | Sprite frame + flip/sketch precompute |
| `animation.h/cpp` | Frame sequence playback |
| `edit.h/cpp` | `ZoomImage`, `FlipImage`, `SketchImage` |
| `alpha.h/cpp` | `putalpha()` — alpha-blended draw |
| `utils.h/cpp` | `my_utils::to_wstring(int)` |

---

## Known Bug Fixes (do not revert)

| File | Fix |
|------|-----|
| `edit.cpp:31` | Alpha check: `& 0xFF000000` (was broken precedence `>> 24`) |
| `character.cpp` | `isAlive()` returns `HP > 0` (was assignment `alive = HP > 0`) |
| `character.cpp` | `getCenterY()` uses `* 0.5` (was integer division) |
| `frame.cpp` | `flag = !flag` (was `flag++`, could overflow signed `short`) |
| `config.h` | `FRAME_BREAK` uses `1000.0` (was integer `1000 / FPS = 16`) |

## Removed Dead Code

- `console.h/cpp` — unused console input thread (had detached thread leak)
- `enemy_atlas.h/cpp` — duplicate of `Enemy` animation loading
- `message.h` — previously removed, then restored for `VK_*` constants
