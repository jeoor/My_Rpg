#ifndef __CONFIG_H__
#define __CONFIG_H__

constexpr inline int ZOOM_RATE = 5;							  // 缩放率
constexpr inline int WINDOWS_W = 750;						  // 窗口宽度
constexpr inline int WINDOWS_H = 750;						  // 窗口高度
constexpr inline int WORLD_W = 2000;						  // 世界宽度
constexpr inline int WORLD_H = 2000;						  // 世界高度
constexpr inline int FPS = 60;								  // 帧率
constexpr inline double FRAME_BREAK = 1000.0 / FPS;			  // 帧间隔
constexpr inline int FRAME_SLEEP_RESERVE_US = 500;			  // 高精度限帧预留时间
constexpr inline double PLAYER_MAX_SPEED = 1.0;				  // 玩家移动速度
constexpr inline double ENEMY_MAX_SPEED = 1.0;				  // 敌人移动速度(与玩家持平)
constexpr inline double PLAYER_ACCELERATION = 0.5;			  // 玩家加速度
constexpr inline double ENEMY_ACCELERATION = 1.0;			  // 敌人加速度
constexpr inline int DELTA_TIME = FPS / 10;					  // 动画帧间隔
constexpr inline int DELTA = 1;								  // 帧更新间隔
constexpr inline double CAMERA_LERP = 0.08;					  // 相机平滑跟随系数
constexpr inline int MAX_ENEMIES = 10;						  // 最大敌人数
constexpr inline int SPAWN_INTERVAL = 5;					  // 敌人生成间隔 (DELTA_TIME 倍数)
constexpr inline int HP_BONUS_INTERVAL = 10;				  // 每杀 N 个敌人回血
constexpr inline int HP_BONUS_AMOUNT = 20;					  // 回血量
constexpr inline int NAV_CLEARANCE = ZOOM_RATE * 3;			  // 寻路网格与障碍的安全距离
constexpr inline int PATH_WAYPOINT_DISTANCE = ZOOM_RATE * 6;  // 路点切换距离
constexpr inline double MOVE_DIRECTION_DEAD_ZONE = ZOOM_RATE; // 朝向切换死区
constexpr inline int MIN_REACHABLE_EDGE_CELLS = 2;			  // 每边最少可出生网格数
constexpr inline int FISH_VISUAL_RANGE = ZOOM_RATE * 14;	  // 敌人感知同伴范围
constexpr inline int FISH_SEPARATION_RANGE = ZOOM_RATE * 8;	  // 敌人最小分离距离
constexpr inline int FISH_STEER_DISTANCE = ZOOM_RATE * 20;	  // 群体转向目标距离
constexpr inline int START_CIRCLE_RADIUS = 80;				  // 开始引导圈半径
constexpr inline int START_TEXT_HEIGHT = 22;				  // START 文本高度

#endif
