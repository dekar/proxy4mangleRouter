#include "httprequest.h"

#include <QTextStream>

HttpRequest::HttpRequest()
{

}

HttpRequest::HttpRequest(const char *buffer, unsigned int size)
{
    QByteArray ba(buffer, size);
    QTextStream stream(ba);
    this->startingLine = stream.readLine();

    QString line = stream.readLine();
    while (!line.isEmpty())
    {
        this->headers.append(line);
        line = stream.readLine();
    }

    this->body = stream.readAll();
}

unsigned int HttpRequest::toBufer(char *buffer)
{
    QByteArray ba;
    QTextStream send(&ba, QIODevice::WriteOnly);
    send << this->startingLine << "\n";
    foreach(QString s, this->headers)
        send << s << "\n";
    send << "\n";
    send << body;
    send.flush();
    memcpy(buffer,ba.data(),ba.size());
    return ba.size();
}

/*void HttpRequest::modifyHost(QString newHost)
{
    //this->headers[0] = "Host: " + newHost;
    this->modifyHeader("Host",newHost);
    // Origin: http://192.168.1.1
    // Referer: http://192.168.1.1/
}*/

bool HttpRequest::modifyHeader(QString header, QString value)
{
    for(int i=0;i<this->headers.size();i++)
    {
        if(this->headers[i].startsWith(header))
        {
            this->headers[i] = header + ": " + value;
            return true;
        }
    }
    return false;
}
