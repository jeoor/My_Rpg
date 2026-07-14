#ifndef __CONFIG_H__
#define __CONFIG_H__

#if __cplusplus >= 201103L							// C++11 及以上
constexpr inline int ZOOM_RATE = 5;					// 缩放率
constexpr inline int WINDOWS_W = 750;				// 窗口宽度
constexpr inline int WINDOWS_H = 750;				// 窗口高度
constexpr inline int WORLD_W = 2000;				// 世界宽度
constexpr inline int WORLD_H = 2000;				// 世界高度
constexpr inline int FPS = 60;						// 帧率
constexpr inline double FRAME_BREAK = 1000.0 / FPS;		// 帧间隔
constexpr inline double PLAYER_MAX_SPEED = 1.0;		// 玩家移动速度
constexpr inline double ENEMY_MAX_SPEED = 0.3;		// 敌人移动速度
constexpr inline double PLAYER_ACCELERATION = 0.5;	// 玩家加速度
constexpr inline double ENEMY_ACCELERATION = 0.3;	// 敌人加速度
constexpr inline int DELTA_TIME = FPS / 10;			// 动画帧间隔
constexpr inline int DELTA = 1;						// 帧更新间隔
constexpr inline double CAMERA_LERP = 0.08;			// 相机平滑跟随系数
constexpr inline int MAX_ENEMIES = 10;				// 最大敌人数
constexpr inline int SPAWN_INTERVAL = 5;			// 敌人生成间隔 (DELTA_TIME 倍数)
constexpr inline int HP_BONUS_INTERVAL = 10;		// 每杀 N 个敌人回血
constexpr inline int HP_BONUS_AMOUNT = 20;			// 回血量

#else
#define WINDOWS_W 750			// 窗口宽度
#define WINDOWS_H 750			// 窗口高度
#define WORLD_W 2000			// 世界宽度
#define WORLD_H 2000			// 世界高度
#define ZOOM_RATE 5				// 缩放率
#define FPS 60					// 帧率
#define FRAME_BREAK (1000.0 / FPS)	// 帧间隔
#define PLAYER_MAX_SPEED 1.0	// 玩家移动速度
#define ENEMY_MAX_SPEED 0.3		// 敌人移动速度
#define PLAYER_ACCELERATION 0.5	// 玩家加速度
#define ENEMY_ACCELERATION 0.3	// 敌人加速度
#define DELTA_TIME FPS / 10		// 动画帧间隔
#define DELTA 1					// 帧更新间隔
#define CAMERA_LERP 0.08		// 相机平滑跟随系数
#define MAX_ENEMIES 10			// 最大敌人数
#define SPAWN_INTERVAL 5		// 敌人生成间隔
#define HP_BONUS_INTERVAL 10	// 回血间隔
#define HP_BONUS_AMOUNT 20		// 回血量

#endif
#endif
