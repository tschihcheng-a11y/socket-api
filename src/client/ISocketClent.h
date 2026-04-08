#pragma once

#include <string>
#include <cstddef>

class ISocketClient
{
public:
    virtual ~ISocketClient() = default;

    virtual bool initialize() = 0;
    virtual bool sendData(const void* data, size_t size) = 0;
    virtual ssize_t receiveData(void* buf, size_t size) = 0;
    virtual void close() = 0;
    virtual bool isInitialized() const = 0;
    virtual std::string getServerAddress() const = 0;
};
