#ifndef CONFIG_H
#define CONFIG_H

#if __cplusplus >= 201402L						// C++14 ������
constexpr inline int WINDOWS_W = 600;			// ���ڿ��
constexpr inline int WINDOWS_H = 600;			// ���ڸ߶�
constexpr inline int ZOOM_RATE = 5;				// ������
constexpr inline int FRAME_BREAK = 1000 / 60;   // ֡��
constexpr inline int Step = 1;					// �ƶ�����
constexpr inline int DELTA_TIME = 6;            // ֡���
constexpr inline int DELTA = 1;					// ֡���¼��

#else
#define WINDOWS_W 500				// ���ڿ��
#define WINDOWS_H 500				// ���ڸ߶�
#define ZOOM_RATE 5					// ������
#define FRAME_BREAK 1000 / 60		// ֡��
#define Step 1						// �ƶ�����
#define DELTA_TIME 6				// ֡���
#define DELTA 1						// ֡���¼��
#endif

#endif