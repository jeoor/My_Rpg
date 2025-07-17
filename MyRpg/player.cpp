#include "player.h"

void Player::init()
{
	// 待机动画
	for (int i = 0; i < 4; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_idle_0" + std::to_wstring(i + 1) + L".png";
		idle[i].set((LPCTSTR)path.c_str(), 32, 16, ZOOM_RATE);
	}
	Animation idleAnimation(idle, 4, 5, 15);

	// 攻击动画
	for (int i = 0; i < 7; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_attack_0" + std::to_wstring(i + 1) + L".png";
		attack[i].set((LPCTSTR)path.c_str(), 48, 24, ZOOM_RATE);
	}
	Animation attackAnimation(attack, 7, 8, 21, false);

	// 跑步动画
	for (int i = 0; i < 8; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_run_0" + std::to_wstring(i + 1) + L".png";
		run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
	}
	Animation runAnimation(run, 8, 8, 14);

	// 设置动画
	animations[0] = idleAnimation;
	animations[1] = attackAnimation;
	animations[2] = runAnimation;
	set(animations, 3);
}

void Player::Idle() { setCurrentAnimation(0); }
void Player::Attack() { setCurrentAnimation(1); }
void Player::Run() { setCurrentAnimation(2); }
void Player::setAttacking(bool isATK) { isAttacking = isATK; }
bool Player::Attacking() const { return isAttacking; }