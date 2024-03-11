#ifndef PROXYHANDLER_H
#define PROXYHANDLER_H
#include <cstring> // Для использования функции memset
#include <iostream>

class ProxyHandler
{
public:
    ProxyHandler();
    static int infinitiLoop(int listenPort, std::string remoteHost, int remotePort);
};

#endif // PROXYHANDLER_H
