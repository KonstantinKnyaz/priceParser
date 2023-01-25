#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class downloader : public QObject
{
    Q_OBJECT
public:
    explicit downloader(QObject *parent = Q_NULLPTR);
    virtual ~downloader();

    void startDownload(QUrl url);

    QByteArray getData();

signals:
    void downloaded();

private slots:
    void  fileDownloaded(QNetworkReply *rpl);

private:
    QNetworkAccessManager m_network;
    QByteArray m_downloadedData;

};

#endif // DOWNLOADER_H
