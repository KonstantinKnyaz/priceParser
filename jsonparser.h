#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonDocument>

class JsonParser
{
public:
    JsonParser();

    int startParse(QString &strJson);

    int startParse(QJsonDocument &doc);

    QString getFilePath();
};

#endif // JSONPARSER_H
