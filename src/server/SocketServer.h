#pragma once

#include <string>
#include <utility>
#include "../platform.h"
#include "../spec.h"
#include "ISocketServer.h"

class SocketServer : public ISocketServer
{
public:
    SocketServer(int port, SocketType type = SocketType::TCP);
    ~SocketServer();

    bool start() override;
    void stop() override;
    bool isRunning() const override;

    // TCP: accept incoming connection, returns {client_fd, "ip:port"}
    std::pair<int, std::string> acceptClient() override;

    // UDP: receive data, fills outClientAddr with "ip:port"
    ssize_t receiveFrom(void* buf, size_t size, std::string& outClientAddr) override;

private:
    int                port_;
    int                server_fd_;
    bool               running_;
    SocketType         type_;
};
