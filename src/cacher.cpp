#include "cacher.h"
#include "api/request.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QXmlStreamReader>

Cacher::Cacher(QObject* parent)
    : QObject(parent)
    , m_songDownloader(new Downloader())
    , m_downloading(false)
{
    connect(m_songDownloader, &Downloader::stringReady, this, &Cacher::saveData);
    connect(m_songDownloader, &Downloader::downloadProgress, this, &Cacher::downloadProgress);
}

void Cacher::setTrack(TrackObject* track)
{
    if (!m_downloading) {
        setArtistId(track->artistId);
        setTrackId(track->trackId);
    }
    m_songDownloader->abort();
}

void Cacher::saveToCache()
{
    QString cachepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + QString::number(m_artistId);
    m_fileToSave = cachepath + "/" + QString::number(m_trackId) + ".mp3";

    QDir cacheDir(cachepath);
    if (!cacheDir.exists()) {
        cacheDir.mkpath(cachepath);
    }

    if (QFile::exists(m_fileToSave)) {
        emit downloadProgress(1);
        emit fileSaved(m_fileToSave);
        return;
    }
    m_downloading = true;
    emit downloadingChanged();

    Request* getTrackDownloadInfoRequest = new Request("/tracks/" + QString::number(m_trackId) + "/download-info");
    getTrackDownloadInfoRequest->get();
    connect(getTrackDownloadInfoRequest, &Request::dataReady, this, &Cacher::getDownloadInfoFinished);
}

QString Cacher::Url()
{
    return m_Url;
}

void Cacher::setTrackId(int id)
{
    if (id != m_trackId) {
        m_trackId = id;
        emit trackIdChanged();
    }
}

void Cacher::setArtistId(int id)
{
    if (id != m_artistId) {
        m_artistId = id;
        emit artistIdChanged();
    }
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

    m_songDownloader->setUrl(finalUrl);
    m_songDownloader->loadData();
}

void Cacher::saveData(QByteArray data)
{
    QFile fileToSave(m_fileToSave);
    fileToSave.open(QFile::ReadWrite);
    fileToSave.write(data);
    fileToSave.close();

    qDebug() << m_fileToSave << "ready";

    m_downloading = false;
    emit downloadingChanged();
    emit fileSaved(m_fileToSave);
}
