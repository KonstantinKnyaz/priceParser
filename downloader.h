#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>

class downloader : public QObject
{
    Q_OBJECT
public:
    explicit downloader(QObject *parent = Q_NULLPTR);
    virtual ~downloader();

    void startDownload(QUrl url);

    QByteArray getData();

signals:
    void downloaded(const QByteArray data);

private slots:
    void  fileDownloaded(QNetworkReply *rpl);

private:
    QNetworkAccessManager m_network;
    QByteArray m_downloadedData;
    QMutex mutex;

};

#endif // DOWNLOADER_H
