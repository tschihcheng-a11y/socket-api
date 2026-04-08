#include <iostream>
#include <string>
#include <cstring>
#include "SocketClient.h"

static void runTcpClient(const std::string& host, int port)
{
    std::cout << "[TCP Client] connecting to " << host << ":" << port << "\n";
    SocketClient client(host, port, SocketType::TCP);

    if (!client.initialize())
    {
        std::cerr << "[TCP Client] failed to initialize\n";
        return;
    }

    // 持續輸入訊息送出，輸入 "exit" 離開
    std::string msg;
    while (true)
    {
        std::cout << "message (exit to quit): ";
        std::getline(std::cin, msg);
        if (msg == "exit")
            break;

        if (!client.sendData(msg.c_str(), msg.size()))
        {
            std::cerr << "[TCP Client] send failed\n";
            break;
        }

        char buf[1024];
        ssize_t n = client.receiveData(buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << "[TCP Client] received: " << buf << "\n";
        }
    }

    client.close();
    std::cout << "[TCP Client] done\n";
}

static void runUdpClient(const std::string& host, int port)
{
    std::cout << "[UDP Client] sending to " << host << ":" << port << "\n";
    SocketClient client(host, port, SocketType::UDP);

    if (!client.initialize())
    {
        std::cerr << "[UDP Client] failed to initialize\n";
        return;
    }

    // 持續輸入訊息送出，輸入 "exit" 離開
    std::string msg;
    while (true)
    {
        std::cout << "message (exit to quit): ";
        std::getline(std::cin, msg);
        if (msg == "exit")
            break;

        if (!client.sendData(msg.c_str(), msg.size()))
        {
            std::cerr << "[UDP Client] send failed\n";
            break;
        }
    }

    client.close();
    std::cout << "[UDP Client] done\n";
}

int main()
{
    std::cout << "select mode:\n"
              << "  1. TCP client\n"
              << "  2. UDP client\n"
              << "choice: ";

    int mode = 0;
    std::cin >> mode;

    std::cout << "host (default 127.0.0.1): ";
    std::string host;
    std::cin >> host;
    if (host.empty())
        host = "127.0.0.1";

    std::cout << "port (default 9000): ";
    int port = 9000;
    std::cin >> port;

    std::cin.ignore(); // 清除換行，避免影響後續 getline

    if (mode == 2)
        runUdpClient(host, port);
    else
        runTcpClient(host, port);

    return 0;
}
