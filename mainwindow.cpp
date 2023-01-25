#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonparser.h"

#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QSettings>

#define URL_LINE    "URL_L"
#define FILE_NAME   "FILE_L"
#define LST_DIR     "DIR"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Игорь чилит");

    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    ui->urlLine->setText(settings.value(URL_LINE, "").toString());
    ui->fullFilePath->setText(settings.value(FILE_NAME, "").toString());
    lastDir = settings.value(LST_DIR, "").toString();

    qDebug() << lastDir;

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

int MainWindow::startReadAndParse(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл!";
        ui->statusbar->showMessage("Не удалось открыть файл!");
        return 1;
    }
    QFileInfo info;
    info.setFile(file);
    QSettings settings(ORGANIZATION_NAME, APP_NAME);
    settings.setValue(LST_DIR, info.filePath());
    QString fileData = file.readAll();
    file.close();
    QStringList urlLst = fileData.split("\n");

    for(int i = 0; i < urlLst.count(); i++) {
        qDebug() << urlLst.at(i);
    }
    return 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    settings.setValue(URL_LINE, ui->urlLine->text());
    settings.setValue(FILE_NAME, ui->fullFilePath->text());
}

void MainWindow::on_choiceFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите текстовый файл с ссылками", lastDir, "Text File (*.txt)");
    if(!fileName.isEmpty())
        ui->fullFilePath->setText(fileName);
}

void MainWindow::on_startFileBtn_clicked()
{
    startReadAndParse(ui->fullFilePath->text());
}
