#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl fileUrl, QString fileName, QObject *parent) :
 QObject(parent)
{
  m_fileName = fileName;
  m_url = fileUrl;
  finished = false;
  connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
 m_DownloadedData = pReply->readAll();
 //emit a signal
 pReply->deleteLater();
 finished = true;
 emit downloaded(m_fileName,m_DownloadedData);
}

void FileDownloader::startDownload()
{
    QNetworkRequest request(m_url);
    m_WebCtrl.get(request);
}


