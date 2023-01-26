#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonparser.h"

#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QSettings>
#include <QMessageBox>

#define URL_LINE    "URL_L"
#define FILE_NAME   "FILE_L"
#define LST_DIR     "DIR"
#define LST_PATH    "PATH"

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
    pathToSave = settings.value(LST_PATH, "").toString();

    qDebug() << lastDir;

    dwnlManager = new downloader(this);

    dwnlManager->connect(dwnlManager, &downloader::downloaded, [this]() {
        QString str = QString::fromStdString(dwnlManager->getData().toStdString());
        JsonParser parser(fullPathToSave);
        int res = parser.startParse(str);
        if(!res) {
//            qDebug() << parser.getFilePath();
            ui->statusbar->showMessage("Парсинг успешно завершился");
        } else {
//            qWarning() << "Парсинг завершился с ошибкой:" << res;
            ui->statusbar->showMessage("Парсинг завершился с ошибкой");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startBtn_clicked()
{
    fullPathToSave = pathToSave;
    if(!fullPathToSave.isNull())
        fullPathToSave.append("/");
    fullPathToSave.append(QString("Result_%1.txt").arg(QDate::currentDate().toString("dd.MM.yy")));
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

    fullPathToSave = pathToSave;

    if(!fullPathToSave.isNull())
        fullPathToSave.append("/");
    fullPathToSave.append(QString("Result_%1.txt").arg(QDate::currentDate().toString("dd.MM.yy")));

    if(QFile::exists(fullPathToSave)) {
        QMessageBox msgBox;
        msgBox.setText("Внимание");
        msgBox.setInformativeText("Файл уже существует\nПерезаписать?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Warning);
        int res = msgBox.exec();
        if(res == QMessageBox::No)
            return FILE_ERROR;
        else if (QMessageBox::Yes) {
            QFile::remove(fullPathToSave);
        }
    }

    for(int i = 0; i < urlLst.count(); i++) {
        qDebug() << urlLst.at(i);
        QUrl url(urlLst.at(i));
        dwnlManager->startDownload(url);
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

void MainWindow::on_pathToSave_triggered()
{
    QString pathLst = pathToSave;
    pathToSave = QFileDialog::getExistingDirectory(this, "Выберите папку", pathToSave);
    if(pathToSave.isEmpty())
        pathToSave = pathLst;
    QSettings settings(ORGANIZATION_NAME, APP_NAME);
    settings.setValue(LST_PATH, pathToSave);
}

void MainWindow::on_quit_triggered()
{
    this->close();
}
