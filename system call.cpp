#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

using namespace std;

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed: " << result << endl;
        return 1;
    }

    SOCKET recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (recvSocket == INVALID_SOCKET) {
        cout << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

  
    sockaddr_in recvAddr;
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_addr.s_addr = INADDR_ANY; 
    recvAddr.sin_port = htons(12345);     

    if (bind(recvSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR) {
        cout << "bind failed: " << WSAGetLastError() << endl;
        closesocket(recvSocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening for UDP packets on port 12345..." << endl;

    char recvBuf[1024];
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    int bytesReceived = recvfrom(
        recvSocket,
        recvBuf,
        sizeof(recvBuf) - 1,  
        0,
        (SOCKADDR*)&senderAddr,
        &senderAddrSize
    );

    if (bytesReceived == SOCKET_ERROR) {
        cout << "recvfrom failed: " << WSAGetLastError() << endl;
    } else {
        recvBuf[bytesReceived] = '\0'; 
        cout << "Received " << bytesReceived << " bytes from "
             << inet_ntoa(senderAddr.sin_addr) << ":" << ntohs(senderAddr.sin_port) << ": "
             << recvBuf << endl;
    }

    closesocket(recvSocket);
    WSACleanup();
    return 0;
}
