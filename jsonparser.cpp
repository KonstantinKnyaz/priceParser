#include "jsonparser.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDate>
#include <QDebug>

///Xlsx lib
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

JsonParser::JsonParser(const QString &path, QObject *parent) : QObject(parent)
  ,
    pathToSave(path)
{
    qDebug() << "Путь до файла с сохранёнными данными:" << pathToSave;
}

JsonParser::~JsonParser()
{

}

int JsonParser::startParse(QString &strJson, PARSEMODE mode)
{
    mutex.lock();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(strJson.toUtf8());
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;

    if(!doc.isObject()) {
        qCritical() << "Входные данные не являются JSON'ом";
        return JSON_ERROR;
    }

    return startParse(doc, mode);
}

int JsonParser::startParse(QJsonDocument &doc, PARSEMODE mode)
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

    if(mode == PARSEMODE::TXT) {

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

    } else if (mode == PARSEMODE::XLSX) {

        qDebug() << "XLSX MODE";

        QXlsx::Document xlsx(pathToSave);

        int colRow = xlsx.dimension().rowCount();

        for(int i = 0; i < array.count(); i++) {
            QJsonObject obj = array.at(i).toObject();
            QString name = obj["name"].toString();
            int price = obj["price"].toInt();
            double col = obj["quantity"].toDouble();

            int row = colRow++;

            xlsx.write(QString("A%1").arg(row), fiscal);
            xlsx.write(QString("B%1").arg(row), adress);
            xlsx.write(QString("C%1").arg(row), name);
            xlsx.write(QString("D%1").arg(row), price);
            xlsx.write(QString("E%1").arg(row), col);
        }
        xlsx.saveAs(pathToSave);
        return NO_ERROR;
    }

    return PARSING_ERROR;
}

QString JsonParser::getFilePath()
{

}
