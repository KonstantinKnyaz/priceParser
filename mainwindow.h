#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "downloader.h"

#define ORGANIZATION_NAME "KostyanTeam"
#define APP_NAME "UrlJsonParser"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();

    void on_choiceFileBtn_clicked();

    void on_startFileBtn_clicked();

    void on_pathToSave_triggered();

    void on_quit_triggered();

private:
    Ui::MainWindow *ui;

    downloader *dwnlManager = Q_NULLPTR;

    QString lastDir;

    QString pathToSave = Q_NULLPTR;

    QString fullPathToSave;

    int startReadAndParse(const QString &fileName);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
