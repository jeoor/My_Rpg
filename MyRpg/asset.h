#ifndef __ASSET_H__
#define __ASSET_H__

#include <unordered_map>
#include <vector>
#include <string>
#include "frame.h"
// 资源管理单例 — 所有 Frame 只加载一次，全局共享
class AssetManager
{
  public:
	static AssetManager &getInstance();
	AssetManager(const AssetManager &) = delete;
	AssetManager &operator=(const AssetManager &) = delete;

	void init();									   // 加载全部资源(游戏启动时调用一次)
	Frame *getFrames(const std::wstring &name);		   // 获取帧数组指针
	int getFrameCount(const std::wstring &name) const; // 获取帧数

  private:
	AssetManager() = default;

	void load(const std::wstring &name, const std::wstring &path, int count, int w, int h,
			  int zoom); // 加载单组帧

	std::unordered_map<std::wstring, std::vector<Frame>> pool;
};
#endif
