#include "SocketServer.h"
#include <cstring>

SocketServer::SocketServer(int port, SocketType type)
    : port_(port),
      server_fd_(-1),
      running_(false),
      type_(type)
{
    socket_init();
}

SocketServer::~SocketServer()
{
    stop();
    socket_cleanup();
}

bool SocketServer::start()
{
    if (running_)
        return false;

    int sockType = (type_ == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    server_fd_ = static_cast<int>(socket(AF_INET, sockType, 0));
    if (server_fd_ < 0)
        return false;

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR,
                   reinterpret_cast<const char*>(&opt), sizeof(opt)) < 0)
    {
        socket_close(server_fd_);
        server_fd_ = -1;
        return false;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(static_cast<u_short>(port_));

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        socket_close(server_fd_);
        server_fd_ = -1;
        return false;
    }

    if (type_ == SocketType::TCP)
    {
        if (listen(server_fd_, 5) < 0)
        {
            socket_close(server_fd_);
            server_fd_ = -1;
            return false;
        }
    }

    running_ = true;
    return true;
}

void SocketServer::stop()
{
    if (!running_)
        return;

    running_ = false;

    if (server_fd_ >= 0)
    {
        socket_close(server_fd_);
        server_fd_ = -1;
    }
}

std::pair<int, std::string> SocketServer::acceptClient()
{
    if (!running_ || type_ != SocketType::TCP)
        return {-1, ""};

    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_fd = static_cast<int>(accept(server_fd_, (struct sockaddr*)&client_address, &client_len));
    if (client_fd < 0)
        return {-1, ""};

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::string client_addr = std::string(client_ip) + ":" + std::to_string(ntohs(client_address.sin_port));

    return {client_fd, client_addr};
}

ssize_t SocketServer::receiveFrom(void* buf, size_t size, std::string& outClientAddr)
{
    if (!running_ || type_ != SocketType::UDP)
        return -1;

    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    ssize_t received = recvfrom(server_fd_, static_cast<char*>(buf), static_cast<int>(size), 0,
                                (struct sockaddr*)&client_address, &client_len);
    if (received < 0)
        return -1;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    outClientAddr = std::string(client_ip) + ":" + std::to_string(ntohs(client_address.sin_port));

    return received;
}

bool SocketServer::isRunning() const
{
    return running_;
}
