#include "jsonparser.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>
#include <QMutexLocker>

JsonParser::JsonParser()
{

}

int JsonParser::startParse(QString &strJson)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(strJson.toUtf8());
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return 1;
    }

    return startParse(doc);
}

int JsonParser::startParse(QJsonDocument &doc)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return 1;
    }

    QJsonObject json = doc.object();
    QJsonArray array = json["items"].toArray();

    for(int i = 0; i < array.count(); i++) {
        QJsonObject obj = array.at(i).toObject();
        QString name = obj["name"].toString();
        int price = obj["price"].toInt();

        qDebug() << "Продукт:" << name << "\nЦена:" << price;
    }

    return 0;
}

QString JsonParser::getFilePath()
{

}
