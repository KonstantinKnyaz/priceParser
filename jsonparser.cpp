#include "jsonparser.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QDate>

JsonParser::JsonParser(const QString &path, QObject *parent) : QObject(parent)
  ,
    pathToSave(path)
{
    qDebug() << "Путь до файла с сохранёнными данными:" << pathToSave;
}

JsonParser::~JsonParser()
{

}

int JsonParser::startParse(QString &strJson)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(strJson.toUtf8());
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return JSON_ERROR;
    }

    return startParse(doc);
}

int JsonParser::startParse(QJsonDocument &doc)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return JSON_ERROR;
    }

    QJsonObject json = doc.object();
    QJsonArray array = json["items"].toArray();
    QString fileName = json["dateTime"].toString();

    QFile file(pathToSave);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCritical() << "Не удалось срздать файл для записи";
        return FILE_ERROR;
    }

    QTextStream stream(&file);

    stream.setCodec("UTF-8");

    stream << file.readAll();

    for(int i = 0; i < array.count(); i++) {
        QJsonObject obj = array.at(i).toObject();
        QString name = obj["name"].toString();
        int price = obj["price"].toInt();

        qDebug() << "Продукт:" << name << "\nЦена:" << price;

        stream << QString("Продукт: %1 \n").arg(name);
        stream << QString("Цена: %1 \n").arg(price);
        stream << "--------------------------------- \n";
    }

    file.close();

    return NO_ERROR;
}

QString JsonParser::getFilePath()
{

}
