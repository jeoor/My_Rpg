#include "enemy_atlas.h"

Atlas::Atlas()
{
	// 待机动画
	for (int i = 0; i < 4; ++i)
	{
		std::wstring path = L"source/characters/mooseman/mooseman_idle_0" + my_utils::to_wstring(i + 1) + L".png";
		idle[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
	}
	Animation idleAnimation(idle, 4, 6, 15);

	// 跑步动画
	for (int i = 0; i < 4; ++i)
	{
		std::wstring path = L"source/characters/mooseman/mooseman_run_0" + my_utils::to_wstring(i + 1) + L".png";
		run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
	}
	Animation runAnimation(run, 4, 5, 14);

	// 攻击动画
	for (int i = 0; i < 6; ++i)
	{
		std::wstring path = L"source/characters/mooseman/mooseman_attack_0" + my_utils::to_wstring(i + 1) + L".png";
		attack[i].set((LPCTSTR)path.c_str(), 20, 16, ZOOM_RATE);
	}
	Animation attackAnimation(attack, 6, 6, 15);

	// 设置动画
	animations[0] = idleAnimation;
	animations[1] = runAnimation;
	animations[2] = attackAnimation;
}