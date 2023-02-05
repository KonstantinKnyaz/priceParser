#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QMutex>

enum ERRORS {
    NO_ERROR =      0x0,
    PARSING_ERROR = 0x1,
    FILE_ERROR  =   0x2,
    JSON_ERROR =    0x3
};

enum PARSEMODE {
    TXT     =   0x0,
    XLSX    =   0x1,
};

class JsonParser : public QObject
{
public:
    explicit JsonParser(const QString &path, QObject *parent = Q_NULLPTR);

    ~JsonParser();

    int startParse(QString &strJson, PARSEMODE mode = TXT);

    int startParse(QJsonDocument &doc, PARSEMODE mode = TXT);

    QString getFilePath();

private:
    QString pathToSave;
    QMutex mutex;
};

#endif // JSONPARSER_H
