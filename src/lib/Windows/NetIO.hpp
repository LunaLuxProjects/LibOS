#pragma once
#include "windows_link.h"
#include <Components/NetIO.h>
#include <iphlpapi.h>
#include <stdexcept>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

struct losSocket_T
{
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in server_address;
    int server_address_size = 0;
    bool isServer = false;
    bool isTCP = false;
};

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

void *winNetworkBytesToSystemBytes(const uint32 *data, const size data_size)
{
    std::vector<uint32> bytes;
    bytes.reserve(data_size);
    for (size i = 0; i < data_size; i++)
        bytes.emplace_back(ntohl(data[i]));
    return std::move(bytes.data());
}

void *winSystemBytesToNetworkBytes(const uint32 *data, const size data_size)
{
    std::vector<uint32> bytes;
    bytes.reserve(data_size);
    for (size i = 0; i < data_size; i++)
        bytes.emplace_back(htonl(data[i]));
    return std::move(bytes.data());
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
    return LOS_SUCCESS;
}

const losResult winCreateSocket(losSocket *socket_in, const losCreateSocketInfo &socket_info)
{
    *socket_in = new losSocket_T();
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_INIT;

    switch (socket_info.socketBits)
    {
    case LOS_SOCKET_TCP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
    case LOS_SOCKET_TCP: {
        (*socket_in)->ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        (*socket_in)->isTCP = true;
        break;
    }
    case LOS_SOCKET_UDP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
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
        SOCKADDR_IN sockAddr = {0};
        sockAddr.sin_port = htons(socket_info.port);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_addr.S_un.S_addr = (*reinterpret_cast<unsigned long *>(ip->h_addr_list[0]));

        if (connect((*socket_in)->ConnectSocket, (SOCKADDR *)(&sockAddr), sizeof(sockAddr)) == SOCKET_ERROR)
        {
            if (isLoaded(true) != LOS_SUCCESS)
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

            return tellError();
        }
    }

    return LOS_SUCCESS;
}

const losResult winReadSocket(losSocket socket, void *data, size *data_size)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    int iResult = 0;
    if (socket->isTCP)
        iResult = recv(socket->ConnectSocket, (char *)data, *data_size, 0);
    else
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    if (iResult == SOCKET_ERROR)
        return tellError();
    else if (iResult == 0)
        return LOS_NET_IO_CONNECTION_CLOSED_SERVER_END;
    else
        return LOS_SUCCESS;
}

const losResult winWriteSocket(losSocket socket, const void *data, const size data_size)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    if (socket->isTCP)
    {
        if (send(socket->ConnectSocket, (const char *)data, data_size, 0) == SOCKET_ERROR)
            return tellError();
    }
    else
    {
        if (sendto(socket->ConnectSocket, (const char *)data, data_size, 0, (struct sockaddr *)&socket->server_address,
                   socket->server_address_size) == SOCKET_ERROR)
            return tellError();
    }
    return LOS_SUCCESS;
}

const losResult winListenSocket(const losCreateSocketServerListenInfo &server_listen)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;

    return LOS_SUCCESS;
}

const losResult winDestorySocket(losSocket socket)
{
    if (shutdown(socket->ConnectSocket, SD_SEND) == SOCKET_ERROR)
    {
        closesocket(socket->ConnectSocket);
        if (isLoaded(true) != LOS_SUCCESS)
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        return LOS_ERROR_COULD_NOT_DESTORY;
    }
    closesocket(socket->ConnectSocket);
    if (isLoaded(true) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    return LOS_SUCCESS;
}