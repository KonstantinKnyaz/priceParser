#include "jsonparser.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDate>
#include <QDebug>

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
    mutex.lock();

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

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return JSON_ERROR;
    }

    QJsonObject json = doc.object();
    QString fiscal = json["fiscalDriveNumber"].toString();
    QJsonArray array = json["items"].toArray();
    QString fileName = json["dateTime"].toString();

    QString adress = json["retailPlaceAddress"].toString();

    QFile file(pathToSave);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCritical() << "Не удалось создать файл для записи";
        return FILE_ERROR;
    }

    QTextStream stream(&file);

    stream.setCodec("UTF-8");

//    stream << file.readAll();

    QByteArray inputData = file.readAll();

#ifdef QT_DEBUG
      stream << "------------------------\n";
#endif

    for(int i = 0; i < array.count(); i++) {
        QJsonObject obj = array.at(i).toObject();
        QString name = obj["name"].toString();
        int price = obj["price"].toInt();
        double col = obj["quantity"].toDouble();

//        qDebug() << "Продукт:" << name << "\nЦена:" << price;

        stream << fiscal << "|" << adress << "|" << name << "|" << price
               << "|" << col << "\n";
    }

    file.close();
    mutex.unlock();

    return NO_ERROR;
}

QString JsonParser::getFilePath()
{

}
