#include "asset.h"
#include "config.h"
#include "utils.h"
AssetManager &AssetManager::getInstance()
{
	static AssetManager instance;
	return instance;
}
void AssetManager::init()
{
	if (!pool.empty())
		return; // 已加载，跳过
	// === 敌人: mooseman ===
	load(L"enemy_idle", L"source/characters/mooseman/mooseman_idle_0", 4, 16, 16, ZOOM_RATE);
	load(L"enemy_run", L"source/characters/mooseman/mooseman_run_0", 4, 16, 16, ZOOM_RATE);
	load(L"enemy_attack", L"source/characters/mooseman/mooseman_attack_0", 6, 20, 16, ZOOM_RATE);

	// === 玩家: basic ===
	load(L"player_idle", L"source/characters/basic/basic_animesword_idle_0", 4, 32, 16, ZOOM_RATE);
	load(L"player_run", L"source/characters/basic/basic_animesword_run_0", 8, 16, 16, ZOOM_RATE);
	load(L"player_attack", L"source/characters/basic/basic_animesword_attack_0", 7, 48, 24, ZOOM_RATE);
}
Frame *AssetManager::getFrames(const std::wstring &name)
{
	auto it = pool.find(name);
	return it != pool.end() ? it->second.data() : nullptr;
}
int AssetManager::getFrameCount(const std::wstring &name) const
{
	auto it = pool.find(name);
	return it != pool.end() ? (int)it->second.size() : 0;
}
void AssetManager::load(const std::wstring &name, const std::wstring &path, int count, int w, int h, int zoom)
{
	std::vector<Frame> frames;
	frames.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		std::wstring full = path + myUtils::toWstring(i + 1) + L".png";
		frames.emplace_back((LPCTSTR)full.c_str(), w, h, zoom);
	}
	pool.insert({name, std::move(frames)});
}
