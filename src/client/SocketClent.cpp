#include "SocketClient.h"
#include <cstring>

SocketClient::SocketClient(const std::string& host, int port, SocketType type)
    : host_(host),
      port_(port),
      socket_fd_(-1),
      initialized_(false),
      type_(type)
{
    socket_init();
}

SocketClient::~SocketClient()
{
    close();
    socket_cleanup();
}

bool SocketClient::initialize()
{
    if (initialized_)
        return true;

    int sockType = (type_ == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    socket_fd_ = static_cast<int>(socket(AF_INET, sockType, 0));
    if (socket_fd_ < 0)
        return false;

    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(static_cast<u_short>(port_));

    if (inet_pton(AF_INET, host_.c_str(), &server_addr_.sin_addr) <= 0)
    {
        socket_close(socket_fd_);
        socket_fd_ = -1;
        return false;
    }

    if (type_ == SocketType::TCP)
    {
        if (connect(socket_fd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) < 0)
        {
            socket_close(socket_fd_);
            socket_fd_ = -1;
            return false;
        }
    }

    initialized_ = true;
    return true;
}

bool SocketClient::sendData(const void* data, size_t size)
{
    if (!initialized_)
        return false;

    ssize_t sent;
    if (type_ == SocketType::TCP)
    {
        sent = send(socket_fd_, static_cast<const char*>(data), static_cast<int>(size), 0);
    }
    else
    {
        sent = sendto(socket_fd_, static_cast<const char*>(data), static_cast<int>(size), 0,
                      (struct sockaddr*)&server_addr_, sizeof(server_addr_));
    }

    if (sent < 0)
        return false;

    return static_cast<size_t>(sent) == size;
}

ssize_t SocketClient::receiveData(void* buf, size_t size)
{
    if (!initialized_)
        return -1;

    if (type_ == SocketType::TCP)
        return recv(socket_fd_, static_cast<char*>(buf), static_cast<int>(size), 0);

    struct sockaddr_in src_addr;
    socklen_t src_len = sizeof(src_addr);
    return recvfrom(socket_fd_, static_cast<char*>(buf), static_cast<int>(size), 0,
                    (struct sockaddr*)&src_addr, &src_len);
}

void SocketClient::close()
{
    if (socket_fd_ >= 0)
    {
        socket_close(socket_fd_);
        socket_fd_ = -1;
        initialized_ = false;
    }
}

bool SocketClient::isInitialized() const
{
    return initialized_;
}

std::string SocketClient::getServerAddress() const
{
    return host_ + ":" + std::to_string(port_);
}
