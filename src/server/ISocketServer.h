#pragma once

#include <string>
#include <utility>
#include <cstddef>

class ISocketServer
{
public:
    virtual ~ISocketServer() = default;

    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;

    // TCP only
    virtual std::pair<int, std::string> acceptClient() = 0;

    // UDP only
    virtual ssize_t receiveFrom(void* buf, size_t size, std::string& outClientAddr) = 0;
};
