#pragma once

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    #ifndef _SSIZE_T_DEFINED
        using ssize_t = int;
        #define _SSIZE_T_DEFINED
    #endif

    inline int socket_close(int fd) { return closesocket(fd); }
    inline int socket_errno()       { return WSAGetLastError(); }

    inline bool socket_init()
    {
        WSADATA wsa;
        return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
    }
    inline void socket_cleanup() { WSACleanup(); }

#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cerrno>

    inline int socket_close(int fd) { return ::close(fd); }
    inline int socket_errno()       { return errno; }
    inline bool socket_init()       { return true; }
    inline void socket_cleanup()    {}
#endif
