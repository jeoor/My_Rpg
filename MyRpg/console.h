#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include "player.h"
#include "enemy.h"

// ����̨�����߳�
void ConsoleInputThread();
void initConsole();
void peekConsole(Player &player);

#endif