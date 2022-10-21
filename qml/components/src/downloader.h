#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkReply>
#include <QObject>

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(QString urlString, QObject* parent = 0);

signals:
    void stringReady(QByteArray answer);
    void urlNotValid();
    void downloadProgress(float progress);

public slots:
    void loadData();

private slots:
    void dataReady(QNetworkReply* reply);
    void onDownloadProgress(qint64 bytesRead, qint64 bytesTotal);

private:
    QNetworkReply* m_response;
    QNetworkAccessManager* m_manager;
    QUrl m_url;
};

#endif // DOWNLOADER_H
