#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
using namespace std;

int cl_count = 0;
struct clients
{
    string order;
    sockaddr_in sender;
};

clients client_order[30];


int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    int senderAddrSize = sizeof(client_order[cl_count].sender);

    cout << "Receiving data...." << endl;
    while (1)
    {
        int time_order = 0;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&client_order[cl_count].sender, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }
        cl_count++;
        receiveBuf[bytesReceived] = '\0';
        cout << "Received from " << client_order[cl_count - 1].sender.sin_addr.s_host << endl;
        cout << "Data: " << receiveBuf << endl;
        string buf = receiveBuf;
        if (buf.find("burger") != -1)
            time_order += 5000;
        if (buf.find("sprite") != -1)
            time_order += 1000;
        if (buf.find("potato") != -1)
            time_order += 3000;
        const size_t sendBufSize = 1024;
        char sendBuf[sendBufSize] = "Please, wait ";
        char sendBuf_1[sendBufSize];
        _itoa(time_order / 1000, sendBuf_1, sendBufSize);
        strcat(sendBuf, sendBuf_1);
        strcat(sendBuf, " seconds\n");
        int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&client_order[cl_count - 1].sender, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }
        _itoa(time_order, sendBuf_1, sendBufSize);
        sendto(udpSocket, sendBuf_1, strlen(sendBuf_1), 0, (SOCKADDR*)&client_order[cl_count - 1].sender, senderAddrSize);
        Sleep(time_order);
        char sendBuf_2[sendBufSize] = "Order is ready! Thanks\n";
        sendto(udpSocket, sendBuf_2, strlen(sendBuf_2), 0, (SOCKADDR*)&client_order[cl_count - 1].sender, senderAddrSize);
    }

    closesocket(udpSocket);
    WSACleanup();
}