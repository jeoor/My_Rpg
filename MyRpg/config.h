#ifndef __CONFIG_H__
#define __CONFIG_H__

#if __cplusplus >= 201103L							// C++11 ������
constexpr inline int ZOOM_RATE = 5;					// ������
constexpr inline int WINDOWS_W = 750;				// ���ڿ��
constexpr inline int WINDOWS_H = 750;				// ���ڸ߶�
constexpr inline int FPS = 60;						// ֡��
constexpr inline double FRAME_BREAK = 1000.0 / FPS;		// ֡���
constexpr inline double PLAYER_MAX_SPEED = 1.0;		// ����ƶ��ٶ�
constexpr inline double ENEMY_MAX_SPEED = 0.3;		// �����ƶ��ٶ�
constexpr inline double PLAYER_ACCELERATION = 0.5;	// ��Ҽ��ٶ�
constexpr inline double ENEMY_ACCELERATION = 0.3;	// ���˼��ٶ�
constexpr inline int DELTA_TIME = FPS / 10;			// ����֡���
constexpr inline int DELTA = 1;						// ֡���¼��

#else
#define WINDOWS_W 750			// ���ڿ��
#define WINDOWS_H 750			// ���ڸ߶�
#define ZOOM_RATE 5				// ������
#define FPS 60					// ֡��
#define FRAME_BREAK 1000 / FPS	// ֡���
#define PLAYER_MAX_SPEED 1.0	// ����ƶ��ٶ�
#define ENEMY_MAX_SPEED 0.3		// �����ƶ��ٶ�
#define PLAYER_ACCELERATION 0.5	// ��Ҽ��ٶ�
#define ENEMY_ACCELERATION 0.3	// ���˼��ٶ�
#define DELTA_TIME FPS / 10		// ����֡���
#define DELTA 1					// ֡���¼��
#endif

#endif