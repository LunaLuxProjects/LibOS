#include "../Cmake.h"
#include <libos/Defines.h>
#include <libos/NetIO.h>
#include <vector>
#if CMAKE_SYSTEM_NUMBER == 0
#    include <arpa/inet.h>
#    include <lstd/ClibLink.h>
#    include <netdb.h>
#    include <sys/socket.h>
#    include <unistd.h>
#    define _ConnectSocket int32 ConnectSocket = 0

losResult tellError()
{
    perror("system error");
    return LOS_NET_IO_CONNECTION_REFUSED;
}
#    define preLoad(x)
#    define sFree close
#    undef SHUT_RDWR
#    define SHUT_RDWR 2
#endif
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#    pragma comment(lib, "Ws2_32.lib")
#    define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#    define WIN32_LEAN_AND_MEAN
#    include <iphlpapi.h>
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
#    define _ConnectSocket SOCKET ConnectSocket = INVALID_SOCKET

const losResult isLoaded(const bool clear)
{
    static WSADATA wsaData;
    static bool wsainit = false;
    if (wsainit)
    {
        if (clear)
        {
            WSACleanup();
            wsainit = false;
        }
        return LOS_SUCCESS;
    }
    else
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            return LOS_ERROR_COULD_NOT_INIT;
        wsainit = true;
    }
    return LOS_SUCCESS;
}

losResult tellError()
{
    int error = WSAGetLastError();
    switch (error)
    {
    case 0x274D: {
        fprintf(stderr, "ERROR: No connection could be made because the target machine actively refused it.\n");
        return LOS_NET_IO_CONNECTION_REFUSED;
    }
    default:
        printf("%x\n", error);
        return LOS_ERROR_COULD_NOT_INIT;
    }
}

#    define preLoad(x)                  \
        if (isLoaded(x) != LOS_SUCCESS) \
            return LOS_ERROR_COULD_NOT_INIT;
#    define sFree closesocket
#    define SHUT_RDWR SD_SEND
#endif

struct losSocket_T
{
    _ConnectSocket;
    struct sockaddr_in server_address = {};
    int server_address_size = 0;
    bool isServer = false;
    bool isTCP = false;
};

uint32 *losNetworkBytesToSystemBytes(const uint32 *data, const data_size size)
{
    std::vector<uint32> bytes;
    bytes.reserve(size);
    for (data_size i = 0; i < size; i++)
        bytes.emplace_back(ntohl(data[i]));
    return std::move(bytes.data());
}

uint32 *losSystemBytesToNetworkBytes(const uint32 *data, const data_size size)
{
    std::vector<uint32> bytes;
    bytes.reserve(size);
    for (data_size i = 0; i < size; i++)
        bytes.emplace_back(htonl(data[i]));
    return std::move(bytes.data());
}

int32 *losNetworkBytesToSystemBytesSigned(const int32 *data, const data_size size)
{
    std::vector<int32> bytes;
    bytes.reserve(size);
    for (data_size i = 0; i < size; i++)
        bytes.emplace_back(ntohl(data[i]));
    return std::move(bytes.data());
}

int32 *losSystemBytesToNetworkBytesSigned(const int32 *data, const data_size size)
{
    std::vector<int32> bytes;
    bytes.reserve(size);
    for (data_size i = 0; i < size; i++)
        bytes.emplace_back(htonl(data[i]));
    return std::move(bytes.data());
}

losResult losCreateSocket(losSocket *socket_in, const losCreateSocketInfo &socket_info)
{
    /* FIXME: this check should stop reusing handles already in use
    if (!(*socket_in))
        return LOS_ERROR_HANDLE_IN_USE;
    */
    *socket_in = new losSocket_T();
    preLoad(false);
    switch (socket_info.socketBits)
    {
    case LOS_SOCKET_TCP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_TCP: {
        (*socket_in)->ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        (*socket_in)->isTCP = true;
        break;
    }
    case LOS_SOCKET_UDP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_UDP: {
        (*socket_in)->server_address.sin_family = AF_INET;               // AF_INET = IPv4 addresses
        (*socket_in)->server_address.sin_port = htons(socket_info.port); // Little to big endian conversion
        inet_pton(AF_INET, socket_info.address,
                  &(*socket_in)->server_address.sin_addr); // Convert from string to byte array
        (*socket_in)->server_address_size = sizeof((*socket_in)->server_address);
        (*socket_in)->ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        (*socket_in)->isTCP = false;
        break;
    }
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }

    if ((*socket_in)->isServer)
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    hostent *ip = gethostbyname(socket_info.address);

    if (ip == NULL)
        return LOS_NET_IO_DOMAIN_NOT_FOUND;

    if ((*socket_in)->isTCP)
    {
        sockaddr_in sockAddr = {0, 0, {}, {}};
        sockAddr.sin_port = htons(socket_info.port);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_addr.s_addr = (*reinterpret_cast<unsigned long *>(ip->h_addr_list[0]));
        if (connect((*socket_in)->ConnectSocket, (sockaddr *)(&sockAddr), sizeof(sockAddr)) < 0)
            return tellError();
    }

    return LOS_SUCCESS;
}
#if CMAKE_COMPILER_NUMBER == 2
#    pragma warning(push)
#    pragma warning(disable : 4244)
#endif
losResult losReadSocket(losSocket socket, void *data, data_size *size)
{
    preLoad(false);
    int iResult = 0;
    if (socket->isTCP)
        iResult = recv(socket->ConnectSocket, (char *)data, *size, 0);
    else
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    if (iResult < 0)
        return tellError();
    else if (iResult == 0)
        return LOS_NET_IO_CONNECTION_CLOSED_SERVER_END;
    else
        return LOS_SUCCESS;
}

losResult losWriteSocket(losSocket socket, const void *data, const data_size size)
{
    preLoad(false);
    if (socket->isTCP)
    {
        if (send(socket->ConnectSocket, (const char *)data, size, 0) < 0)
            return tellError();
    }
    else
    {
        if (sendto(socket->ConnectSocket, (const char *)data, size, 0, (struct sockaddr *)&socket->server_address,
                   socket->server_address_size) < 0)
            return tellError();
    }
    return LOS_SUCCESS;
}
#if CMAKE_COMPILER_NUMBER == 2
#    pragma warning(pop)
#endif

losResult losListenSocket(const losCreateSocketServerListenInfo &)
{
    preLoad(false);
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losDestorySocket(losSocket socket)
{
    if (socket->isTCP)
    {
        if (shutdown(socket->ConnectSocket, SHUT_RDWR) < 0)
        {
            sFree(socket->ConnectSocket);
            preLoad(true);
            return LOS_ERROR_COULD_NOT_DESTORY;
        }
    }
    sFree(socket->ConnectSocket);
    preLoad(true);
    return LOS_SUCCESS;
}