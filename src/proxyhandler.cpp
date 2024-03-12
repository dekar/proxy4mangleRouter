#include "proxyhandler.h"
#include <arpa/inet.h> // Для использования функции inet_pton
#include <sys/socket.h> // Для использования структуры sockaddr_in
#include <netinet/in.h> // Для использования структуры sockaddr_in
#include <unistd.h> // Для использования функции close


#include <thread>
#include <stdexcept>
constexpr int BUFFER_SIZE = 128*1024;
int mangleSending(char *buffer, int size);
int mangleRecieve(char *buffer, int size);

ProxyHandler::ProxyHandler()
{

}

void proxyClient(int clientSocket, const std::string& remoteHost, int remotePort) {
    struct sockaddr_in remoteAddr;
    std::memset(&remoteAddr, 0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    inet_pton(AF_INET, remoteHost.c_str(), &(remoteAddr.sin_addr));
    remoteAddr.sin_port = htons(remotePort);

    int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (remoteSocket < 0) {
        std::cerr << "Failed to create remote socket" << std::endl;
        return;
    }

    if (connect(remoteSocket, reinterpret_cast<struct sockaddr*>(&remoteAddr), sizeof(remoteAddr)) < 0) {
        std::cerr << "Failed to connect to remote server" << std::endl;
        close(remoteSocket);
        return;
    }

    char bufferIn[BUFFER_SIZE];
    char bufferOut[BUFFER_SIZE];
    ssize_t bytesRead;

    usleep(100*1000);

    while((bytesRead = recv(clientSocket, bufferIn, BUFFER_SIZE, 0)) == 0);
    bytesRead = mangleSending(bufferIn, bytesRead);
    send(remoteSocket, bufferIn, bytesRead, 0);
    int bufPos = 0;
    while((bytesRead = recv(remoteSocket, bufferOut + bufPos, BUFFER_SIZE - bufPos, 0)) >0) //while not closed
    {
        bufPos += bytesRead;
    };
    bytesRead = mangleRecieve(bufferOut, bufPos);
    send(clientSocket, bufferOut, bytesRead, 0);
    close(clientSocket);
    std::cerr << "Got "<< bytesRead << " bytes" << std::endl;
    usleep(100*1000);
}


int ProxyHandler::infinitiLoop(int listenPort, std::string remoteHost, int remotePort)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(listenPort);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Proxy server running on port " << listenPort << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        std::thread(proxyClient, clientSocket, remoteHost, remotePort).detach();
    }

    close(serverSocket);

}

