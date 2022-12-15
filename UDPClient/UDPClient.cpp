#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
using namespace std;

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

    sockaddr_in addrTo;
    addrTo.sin_family = AF_INET;
    addrTo.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.0.1", &addrTo.sin_addr.s_addr);

    string sendBuf;
    cout << "Send data" << endl;
    cin.ignore();
    getline(cin, sendBuf, '\n');
    int sendResult = sendto(udpSocket, sendBuf.c_str(), strlen(sendBuf.c_str()), 0, (SOCKADDR*)&addrTo, sizeof(addrTo));
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in addrFrom;
    int addrFromSize = sizeof(addrFrom);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    receiveBuf[bytesReceived] = '\0';
    cout << "Received from " << addrFrom.sin_addr.s_host << endl;
    cout << "Data: " << receiveBuf << endl;
    recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);
    receiveBuf[bytesReceived] = '\0';
    int time_w = (atoi(receiveBuf) + 1) * 1000;
    cout << endl << time_w;
    Sleep(time_w);
    recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);
    receiveBuf[bytesReceived] = '\0';
    closesocket(udpSocket);
    WSACleanup();
}
