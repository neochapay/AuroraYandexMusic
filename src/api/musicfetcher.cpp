/*
 * Copyright (C) 2023-2024 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "musicfetcher.h"
#include "request.h"

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QXmlStreamReader>

MusicFetcher::MusicFetcher(QObject* parent)
    : QObject(parent)
{
    m_settings = new Settings();
    QDir cacheDir;
    cacheDir.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cachedMusic/");
}

void MusicFetcher::load(Track* track)
{
    if (track == nullptr) {
        qWarning() << "Track to download is null";
        return;
    }

    if(track->trackId() < 1) {
        qWarning() << "TrackID to download is wrong";
        return;
    }

    if(track->filePath().isEmpty()) {
        qWarning() << "Track filepath is wrong";
        return;
    }

    m_track = track;

    m_linksList.clear();

    QFile trackFile(m_track->filePath());
    if (trackFile.exists()) {
        qDebug() << "File exists" << m_track->downloaded() << m_track->filePath();
        emit trackReady(m_track->filePath());
    } else {
        Request* downloadInfoRequest = new Request("/tracks/" + QString::number(m_track->trackId()) + "/download-info");
        connect(downloadInfoRequest, &Request::dataReady, this, &MusicFetcher::downloadInfoHandler);

        downloadInfoRequest->get();
    }
}

void MusicFetcher::downloadInfoHandler(QJsonValue value)
{
    QJsonArray linksArray = value.toArray();

    for (int i = 0; i < linksArray.count(); i++) {
        QJsonObject linksObject = linksArray.at(i).toObject();
        DownloadLink* dLink = new DownloadLink;

        dLink->codec = linksObject.value("codec").toString();
        dLink->gain = linksObject.value("gain").toBool();
        dLink->preview = linksObject.value("preview").toBool();
        dLink->downloadInfoUrl = linksObject.value("downloadInfoUrl").toString();
        dLink->direct = linksObject.value("direct").toBool();
        dLink->bitrateInKbps = linksObject.value("bitrateInKbps").toInt();

        m_linksList.push_back(dLink);
    }

    if (m_linksList.isEmpty()) {
        emit error("Cant get download links");

        return;
    }

    QString downloadInfoUrl;
    if (m_linksList.length() == 1) {
        downloadInfoUrl = m_linksList.first()->downloadInfoUrl;
    } else {
        bool useHD = m_settings->value("useHD", true).toBool();
        int kbs = 0;
        for (const DownloadLink* link : m_linksList) {
            if (link->codec == "aac") {
                // ACC is broken
                continue;
            }

            if (kbs == 0) {
                kbs = link->bitrateInKbps;
                downloadInfoUrl = link->downloadInfoUrl;
            } else {
                if (useHD) {
                    if (kbs < link->bitrateInKbps) {
                        kbs = link->bitrateInKbps;
                        downloadInfoUrl = link->downloadInfoUrl;
                    }
                } else {
                    if (kbs > link->bitrateInKbps) {
                        kbs = link->bitrateInKbps;
                        downloadInfoUrl = link->downloadInfoUrl;
                    }
                }
            }
        }
    }

    if (downloadInfoUrl.isEmpty()) {
        emit error("Cant get download links");
        return;
    }

    QNetworkAccessManager* downloadInfoManager = new QNetworkAccessManager(this);
    QNetworkRequest downloadInfoUrlRequest(downloadInfoUrl);
    QNetworkReply* reply = downloadInfoManager->get(downloadInfoUrlRequest);

    connect(reply, &QNetworkReply::finished, this, &MusicFetcher::downloadInfoUrlHandler);
}

void MusicFetcher::downloadInfoUrlHandler()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    QString answerXML = reply->readAll();
    delete  reply;

    QString host;
    QString path;
    QString ts;
    QString region;
    QString s;

    QXmlStreamReader reader(answerXML);
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNext() == QXmlStreamReader::StartElement) {
            if (reader.name() == QString("host")) {
                host = reader.readElementText();
            }
            if (reader.name() == QString("path")) {
                path = reader.readElementText();
            }
            if (reader.name() == QString("ts")) {
                ts = reader.readElementText();
            }
            if (reader.name() == QString("region")) {
                region = reader.readElementText();
            }
            if (reader.name() == QString("s")) {
                s = reader.readElementText();
            }
        }
    }

    QString sign = QString(QCryptographicHash::hash((("XGRlBW9FXlekgbPrRHuSiA" + path.mid(1) + s).toUtf8()), QCryptographicHash::Md5).toHex());
    QString finalUrl = "https://" + host + "/get-mp3/" + sign + "/" + ts + path;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(finalUrl);
    QNetworkReply* m_reply = manager->get(request);

    connect(manager, &QNetworkAccessManager::finished, this, &MusicFetcher::dataReadyHandler);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &MusicFetcher::onDownloadProgress);
}

void MusicFetcher::dataReadyHandler(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
    }

    QFile fileToSave(m_track->filePath());
    fileToSave.open(QFile::ReadWrite);
    fileToSave.write(reply->readAll());
    fileToSave.close();

    emit trackReady(m_track->filePath());
    m_linksList.clear();
}

void MusicFetcher::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        float progress;
        progress = (float)bytesRead / (float)bytesTotal;
        emit downloadProgress(progress);
    }
}
