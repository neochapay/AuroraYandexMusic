#include "cacher.h"
#include "apirequest.h"
#include "authorization.h"
#include "downloader.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QXmlStreamReader>

Cacher::Cacher(Track* track, QObject* parent)
    : QObject(parent)
{
    m_track = track;
}

void Cacher::saveToCache()
{
    QString cachepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + QString::number(m_track->artistId);
    m_fileToSave = cachepath + "/" + QString::number(m_track->trackId) + ".mp3";

    QDir cacheDir(cachepath);
    if (!cacheDir.exists()) {
        cacheDir.mkpath(cachepath);
    }

    if (QFile::exists(m_fileToSave)) {
        return;
    }

    ApiRequest* getTrackDownloadInfoRequest = new ApiRequest();
    QUrlQuery query;
    getTrackDownloadInfoRequest->makeApiGetRequest("/tracks/" + QString::number(m_track->trackId) + "/download-info", query);
    connect(getTrackDownloadInfoRequest, &ApiRequest::gotResponse, this, &Cacher::getDownloadInfoFinished);
}

QString Cacher::fileToSave()
{
    return m_fileToSave;
}

QString Cacher::Url()
{
    return m_Url;
}

void Cacher::getDownloadInfoFinished(const QJsonValue& value)
{
    QJsonArray qja = value.toArray();

    int bitrateInKbps = 0;
    QString downloadInfoUrl;

    foreach (const QJsonValue& value, qja) {
        if (value.toObject()["codec"].toString() == "mp3" && value.toObject()["bitrateInKbps"].toInt() > bitrateInKbps) {
            bitrateInKbps = value.toObject()["bitrateInKbps"].toInt();
            downloadInfoUrl = value.toObject()["downloadInfoUrl"].toString();
        }
    }

    if (downloadInfoUrl.isEmpty()) {
        return;
    }

    QNetworkAccessManager* dInfoManager = new QNetworkAccessManager(this);
    QNetworkRequest nr(downloadInfoUrl);
    Authorization::setupRequest(&nr);
    QNetworkReply* reply = dInfoManager->get(nr);

    connect(reply, &QNetworkReply::finished, this, &Cacher::getSongUrl);
}

void Cacher::getSongUrl()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    QString DataAsString2 = reply->readAll();

    QString host;
    QString path;
    QString ts;
    QString region;
    QString s;

    QXmlStreamReader reader(DataAsString2);
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNext() == QXmlStreamReader::StartElement) {
            if (reader.name() == "host") {
                host = reader.readElementText();
            }
            if (reader.name() == "path") {
                path = reader.readElementText();
            }
            if (reader.name() == "ts") {
                ts = reader.readElementText();
            }
            if (reader.name() == "region") {
                region = reader.readElementText();
            }
            if (reader.name() == "s") {
                s = reader.readElementText();
            }
        }
    }

    QString sign = QString(QCryptographicHash::hash((("XGRlBW9FXlekgbPrRHuSiA" + path.mid(1) + s).toUtf8()), QCryptographicHash::Md5).toHex());
    QString finalUrl = "https://" + host + "/get-mp3/" + sign + "/" + ts + path;
    m_Url = finalUrl;
    Downloader* songDownloader = new Downloader(finalUrl);
    connect(songDownloader, &Downloader::stringReady, this, &Cacher::saveData);
    songDownloader->loadData();
}

void Cacher::saveData(QByteArray data)
{
    QFile fileToSave(m_fileToSave);
    fileToSave.open(QFile::ReadWrite);
    fileToSave.write(data);
    fileToSave.close();

    qDebug() << m_fileToSave << "ready";
}
