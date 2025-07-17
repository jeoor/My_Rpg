#ifndef CONFIG_H
#define CONFIG_H

#if __cplusplus >= 201402L						// C++14 及以上
constexpr inline int WINDOWS_W = 600;			// 窗口宽度
constexpr inline int WINDOWS_H = 600;			// 窗口高度
constexpr inline int ZOOM_RATE = 5;				// 缩放率
constexpr inline int FRAME_BREAK = 1000 / 60;   // 帧数
constexpr inline int Step = 1;					// 移动步长
constexpr inline int DELTA_TIME = 6;            // 帧间隔
constexpr inline int DELTA = 1;					// 帧更新间隔

#else
#define WINDOWS_W 500				// 窗口宽度
#define WINDOWS_H 500				// 窗口高度
#define ZOOM_RATE 5					// 缩放率
#define FRAME_BREAK 1000 / 60		// 帧数
#define Step 1						// 移动步长
#define DELTA_TIME 6				// 帧间隔
#define DELTA 1						// 帧更新间隔
#endif

#endif