#ifndef __CONFIG_H__
#define __CONFIG_H__

#if __cplusplus >= 201103L						// C++11 ������
constexpr inline int WINDOWS_W = 750;			// ���ڿ��
constexpr inline int WINDOWS_H = 750;			// ���ڸ߶�
constexpr inline int ZOOM_RATE = 5;				// ������
constexpr inline int FPS = 60;					// ֡��
constexpr inline int FRAME_BREAK = 1000 / FPS;  // ֡���
constexpr inline int Step = 1;					// �ƶ�����
constexpr inline int DELTA_TIME = 5;            // ����֡���
constexpr inline int DELTA = 1;					// ֡���¼��

#else
#define WINDOWS_W 750				// ���ڿ��
#define WINDOWS_H 750				// ���ڸ߶�
#define ZOOM_RATE 5					// ������
#define FPS 60						// ֡��
#define FRAME_BREAK 1000 / FPS		// ֡���
#define Step 1						// �ƶ�����
#define DELTA_TIME 5				// ����֡���
#define DELTA 1						// ֡���¼��
#define inline

#endif

#endif