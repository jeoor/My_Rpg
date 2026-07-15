# Repository Guidelines

## Project Structure & Module Organization

This repository is a Windows C++ RPG built as a Visual Studio application. Gameplay code and headers live in `MyRpg/`. Keep related declarations and implementations paired, such as `enemy.h`/`enemy.cpp` and `sceneManager.h`/`sceneManager.cpp`. `rpgMain.cpp` is the entry point; scene classes (`startScene`, `gameScene`, and `endScene`) control major game states. Shared configuration belongs in `MyRpg/config.h`, while reusable helpers belong in `MyRpg/utils.*`.

Runtime assets are grouped under `MyRpg/source/`: `characters/`, `tilesets/`, `items/`, `fonts/`, `icon/`, and `mus/`. Treat `MyRpg/x64/` as generated build output and do not commit new binaries from it. Visual Studio project metadata is stored in `MyRpg/MyRpg.vcxproj` and `MyRpg/MyRpg.vcxproj.filters`.

## Build, Test, and Development Commands

Run commands from a Developer PowerShell for Visual Studio:

```powershell
msbuild .\MyRpg\MyRpg.vcxproj /p:Configuration=Debug /p:Platform=x64
msbuild .\MyRpg\MyRpg.vcxproj /p:Configuration=Release /p:Platform=x64
.\MyRpg\x64\Debug\MyRpg.exe
```

The project uses MSVC toolset `v145`, C++20, and `winmm.lib`. Use Debug for iteration and Release for final verification. If command-line MSBuild is unavailable, open `MyRpg/MyRpg.vcxproj` in Visual Studio and build the matching configuration.

## Coding Style & Naming Conventions

Use tabs for indentation and Allman braces. Keep exactly one blank line after the include block and no blank line between adjacent function definitions. Files, functions, local variables, and data members use lower camel case: `gameScene.cpp`, `updateState()`, `currentSpeed`. Class and enum type names use PascalCase: `GameScene`, `SceneManager`. `constexpr inline` configuration constants and macros use upper snake case, such as `WINDOWS_W` and `PATH_WAYPOINT_DISTANCE`. Prefer RAII, standard-library containers, and explicit ownership over raw allocation. Keep asset paths relative to the executable and preserve UTF-8 source encoding. Run `clang-format -i MyRpg/*.cpp MyRpg/*.h` before submitting code changes.

## Testing Guidelines

There is currently no automated test framework or coverage requirement. Every change must compile in x64 Debug and be exercised manually. For gameplay changes, verify scene transitions, movement, collision, enemy pathfinding, camera behavior, and asset loading. If tests are introduced, place them in `tests/`, name files after the unit under test (for example, `collision_test.cpp`), and document the runner here.

## Commit & Pull Request Guidelines

Recent history uses short prefixes such as `feat:` and `fix:`; follow that pattern with an imperative, scoped summary. Keep commits single-purpose and exclude generated output. Pull requests should explain behavior changes, list manual test steps, link relevant issues, and include screenshots or a short capture for visible gameplay or UI changes.
