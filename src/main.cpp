#include <cstring> // Для использования функции memset
#include <iostream>
#include <QByteArray>
#include "proxyhandler.h"
#include "httprequest.h"

QList<QPair<HttpRequest, HttpRequest> > pairs;

int mangleSending(char *buffer, int size)
{
    HttpRequest rq(buffer, size);
    rq.modifyHeader("Host","192.168.1.1");
    rq.modifyHeader("Origin","http://192.168.1.1");
    rq.modifyHeader("Referer","http://192.168.1.1/");
    pairs.append(QPair<HttpRequest, HttpRequest>(rq, HttpRequest()));
    return rq.toBufer(buffer);
//host: 127.0.0.1:8808\r\n ->
}
int mangleRecieve(char *buffer, int size)
{
    HttpRequest rq(buffer, size);
    pairs.last().second = rq;
    rq.modifyBody("var digitFlag ;","return true;");
    return rq.toBufer(buffer);
}
void test()
{
    char vogon_poemх[] = R"V0G0N(POST /submit-form HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded
Content-Length: 27
Connection: keep-alive

body_line1
body_line2
body_line3)V0G0N";
    int sizeIn = sizeof(vogon_poemх);
    char res [sizeof(vogon_poemх)];
    HttpRequest rq(vogon_poemх,sizeof(vogon_poemх));
    int sizeOut = rq.toBufer(res);
    exit(0);
}

int main(int argc, char* argv[]) {
    //test();
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <port> <remote_host> <remote_port>" << std::endl;
        return 1;
    }

    ProxyHandler::infinitiLoop(std::stoi(argv[1]), argv[2],std::stoi(argv[3]));

    return 0;
}
