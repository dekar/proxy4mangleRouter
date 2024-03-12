#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QString>
#include <QStringList>

class HttpRequest
{
public:
    HttpRequest();
    HttpRequest(const char* buffer, unsigned int size);
    QString startingLine;
    QStringList headers;
    QString body;
    unsigned int toBufer(char *buffer);


    bool modifyHeader(QString header, QString value);
    bool modifyBody(QString findFor, QString replaceTo);
};

#endif // HTTPREQUEST_H
