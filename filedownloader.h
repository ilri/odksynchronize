#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl fileUrl, QString fileName,QObject *parent = 0);
  virtual ~FileDownloader();
  bool finished;
  void startDownload();

 signals:
  void downloaded(QString,QByteArray);

 private slots:
  void fileDownloaded(QNetworkReply* pReply);

 private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
  QString m_fileName;
  QUrl m_url;
};

#endif // FILEDOWNLOADER_H
