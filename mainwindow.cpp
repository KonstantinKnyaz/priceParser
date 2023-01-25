#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonparser.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dwnlManager = new downloader(this);

    dwnlManager->connect(dwnlManager, &downloader::downloaded, [this]() {
        QString str = QString::fromStdString(dwnlManager->getData().toStdString());
        JsonParser parser;
        int res = parser.startParse(str);
        if(!res) {
//            qDebug() << parser.getFilePath();
        } else {
//            qWarning() << "Парсинг завершился с ошибкой:" << res;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startBtn_clicked()
{
    QUrl url(ui->urlLine->text());
    dwnlManager->startDownload(url);
}
