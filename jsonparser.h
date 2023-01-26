#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonDocument>

enum ERRORS {
    NO_ERROR =      0x0,
    PARSING_ERROR = 0x1,
    FILE_ERROR  =   0x2,
    JSON_ERROR =    0x3
};

class JsonParser : public QObject
{
public:
    explicit JsonParser(const QString &path, QObject *parent = Q_NULLPTR);

    ~JsonParser();

    int startParse(QString &strJson);

    int startParse(QJsonDocument &doc);

    QString getFilePath();

private:
    QString pathToSave;

};

#endif // JSONPARSER_H
