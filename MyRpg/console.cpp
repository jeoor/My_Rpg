#include "console.h"

// ȫ�ֱ�������
std::string g_command;
std::atomic<bool> g_has_command{false};
std::mutex g_command_mutex;

// ����̨�����߳�
void ConsoleInputThread()
{
    std::string cmd;
    while (true)
    {
        std::cout << ">> "; // �����ʾ��
        std::getline(std::cin, cmd);
        {
            std::lock_guard<std::mutex> lock(g_command_mutex);
            g_command = cmd;
            g_has_command = true;
        }
    }
}

void initConsole()
{
    // ��������̨�����߳�
    std::thread inputThread(ConsoleInputThread);
    inputThread.detach();
}

void peekConsole(Player &player)
{
    // ����Ƿ�����ָ��
    if (g_has_command.exchange(false))
    {
        std::cin.clear();
        std::string cmd;
        {
            std::lock_guard<std::mutex> lock(g_command_mutex);
            cmd = g_command;
        }
        if (cmd == "ls")
        {
            std::cout << player.getCurrentAnimation() << std::endl;
            std::cout << player.getX() << ", " << player.getY() << std::endl;
        }
        else if (cmd == "cls" || cmd == "clean")
        {
            system("cls"); // ����
        }
        else if (cmd == "help")
        {
            std::cout << "Available commands: exit, ls, help" << std::endl;
        }
        else if (cmd == "attack")
        {
            player.setAttacking(true);
        }
        else if (cmd == "flip")
        {
            player.changeFlip();
            if (player.getDir() == Character::dir::right)
            {
                player.setDir(Character::dir::left);
            }
            else
            {
                player.setDir(Character::dir::right);
            }
        }
        else
        {
            std::cout << "Unknown command: '" << cmd << "'" << std::endl;
            std::cout << "Type 'help' for available commands." << std::endl;
        }
    }
}