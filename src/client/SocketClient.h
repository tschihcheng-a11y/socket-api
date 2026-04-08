#pragma once

#include <string>
#include "../platform.h"
#include "../spec.h"
#include "ISocketClent.h"

class SocketClient : public ISocketClient
{
public:
    SocketClient(const std::string& host, int port, SocketType type = SocketType::UDP);
    ~SocketClient();

    bool initialize();
    bool sendData(const void* data, size_t size);
    ssize_t receiveData(void* buf, size_t size);
    void close();
    bool isInitialized() const;
    std::string getServerAddress() const;

private:
    std::string        host_;
    int                port_;
    int                socket_fd_;
    bool               initialized_;
    SocketType         type_;
    struct sockaddr_in server_addr_;
};
