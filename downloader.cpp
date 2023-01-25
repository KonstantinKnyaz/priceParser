#include "downloader.h"

downloader::downloader(QObject *parent) : QObject(parent)
{
    connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
}

downloader::~downloader()
{

}

void downloader::startDownload(QUrl url)
{
   QNetworkRequest request;

   QSslConfiguration sSlConfig;
   sSlConfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
   sSlConfig.setProtocol(QSsl::TlsV1_2);

   request.setSslConfiguration(sSlConfig);
   request.setUrl(url);
   m_network.get(request);

   qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

}

QByteArray downloader::getData()
{
    return m_downloadedData;
}

void downloader::fileDownloaded(QNetworkReply *rpl)
{
    m_downloadedData = rpl->readAll();
    rpl->deleteLater();
    emit downloaded();
}
