#include "sceneManager.h"
#include "startScene.h"
#include "gameScene.h"
#include "endScene.h"
#include <chrono>
#include <thread>
#pragma comment(lib, "Winmm.lib")
class TimerResolutionGuard
{
  public:
	TimerResolutionGuard() { timeBeginPeriod(1); }
	~TimerResolutionGuard() { timeEndPeriod(1); }
	TimerResolutionGuard(const TimerResolutionGuard &) = delete;
	TimerResolutionGuard &operator=(const TimerResolutionGuard &) = delete;
};
std::unique_ptr<Scene> SceneManager::createScene(GameState state)
{
	switch (state)
	{
	case GameState::Start:
		return std::make_unique<StartScene>();
	case GameState::Playing:
		return std::make_unique<GameScene>();
	case GameState::GameOver:
		return std::make_unique<EndScene>(score);
	default:
		return nullptr;
	}
}
void SceneManager::transitionTo(GameState state)
{
	if (currentScene)
		currentScene->onExit();
	switch (state)
	{
	case GameState::Quit:
		running = false;
		currentScene = nullptr;
		return;
	default:
		currentScene = createScene(state);
		currentState = state;
		if (currentScene)
			currentScene->onEnter();
	}
}
void SceneManager::run()
{
	TimerResolutionGuard timerResolution;

	using FrameClock = std::chrono::steady_clock;
	constexpr auto frameDuration = std::chrono::duration<double, std::milli>(1000.0 / FPS);
	constexpr auto sleepReserve = std::chrono::microseconds(FRAME_SLEEP_RESERVE_US);
	auto nextFrame = FrameClock::now();
	auto fpsTimer = nextFrame;
	int frameCount = 0;

	transitionTo(GameState::Start);
	while (running && currentScene)
	{
		nextFrame += std::chrono::duration_cast<FrameClock::duration>(frameDuration);

		GameState next = currentScene->onFrame(*this);
		if (next != GameState::Running)
		{
			transitionTo(next);
			if (!currentScene)
				break;
		}

		FlushBatchDraw();

		auto now = FrameClock::now();
		if (now < nextFrame)
		{
			if (now + sleepReserve < nextFrame)
				std::this_thread::sleep_until(nextFrame - sleepReserve);
			while (FrameClock::now() < nextFrame)
				std::this_thread::yield();
		}
		else if (now - nextFrame > frameDuration)
		{
			// 严重掉帧后重新同步，避免连续追赶造成更多抖动。
			nextFrame = now;
		}

		frameCount++;
		now = FrameClock::now();
		if (now - fpsTimer >= std::chrono::seconds(1))
		{
			currentFps = frameCount;
			frameCount = 0;
			fpsTimer = now;
		}
	}
}
void SceneManager::setScore(int s) { score = s; }
int SceneManager::getScore() const { return score; }
int SceneManager::getFps() const { return currentFps; }
