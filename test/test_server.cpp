#include <iostream>
#include <string>
#include <cstring>
#include "SocketServer.h"

static void runTcpServer(int port)
{
    std::cout << "[TCP Server] starting on port " << port << "\n";
    SocketServer server(port, SocketType::TCP);

    if (!server.start())
    {
        std::cerr << "[TCP Server] failed to start\n";
        return;
    }

    // 持續接受新 client，直到 Ctrl+C
    while (server.isRunning())
    {
        std::cout << "[TCP Server] waiting for client...\n";

        auto [client_fd, addr] = server.acceptClient();
        if (client_fd < 0)
        {
            std::cerr << "[TCP Server] accept failed\n";
            break;
        }
        std::cout << "[TCP Server] client connected: " << addr << "\n";

        // 持續接收該 client 的資料，直到對方斷線
        char buf[1024];
        while (true)
        {
            ssize_t n = recv(client_fd, buf, sizeof(buf) - 1, 0);
            if (n <= 0)
            {
                // n == 0 表示 client 正常斷線，n < 0 表示錯誤
                std::cout << "[TCP Server] client disconnected: " << addr << "\n";
                break;
            }
            buf[n] = '\0';
            std::cout << "[TCP Server] received: " << buf << "\n";

            const char* reply = "Hello from TCP server";
            send(client_fd, reply, static_cast<int>(strlen(reply)), 0);
        }

        socket_close(client_fd);
        // 回到外層 while，繼續等下一個 client
    }

    server.stop();
    std::cout << "[TCP Server] done\n";
}

static void runUdpServer(int port)
{
    std::cout << "[UDP Server] starting on port " << port << "\n";
    SocketServer server(port, SocketType::UDP);

    if (!server.start())
    {
        std::cerr << "[UDP Server] failed to start\n";
        return;
    }

    // 持續接收封包
    while (server.isRunning())
    {
        char buf[1024];
        std::string clientAddr;
        ssize_t n = server.receiveFrom(buf, sizeof(buf) - 1, clientAddr);
        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << "[UDP Server] received from " << clientAddr << ": " << buf << "\n";
        }
    }

    server.stop();
    std::cout << "[UDP Server] done\n";
}

int main()
{
    std::cout << "select mode:\n"
              << "  1. TCP server\n"
              << "  2. UDP server\n"
              << "choice: ";

    int mode = 0;
    std::cin >> mode;

    std::cout << "port (default 9000): ";
    int port = 9000;
    std::cin >> port;

    if (mode == 2)
        runUdpServer(port);
    else
        runTcpServer(port);

    return 0;
}
