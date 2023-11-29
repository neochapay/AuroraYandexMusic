#include "searchmodel.h"
#include "../api/request.h"
#include "../cacher.h"
#include "../trackobject.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>
#include <QUrlQuery>

SearchModel::SearchModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_loading(false)
    , m_currentIndex(-1)
{
    m_hash.insert(Qt::UserRole, QByteArray("trackId"));
    m_hash.insert(Qt::UserRole + 1, QByteArray("artistId"));
    m_hash.insert(Qt::UserRole + 2, QByteArray("artistName"));
    m_hash.insert(Qt::UserRole + 3, QByteArray("artistCover"));
    m_hash.insert(Qt::UserRole + 4, QByteArray("albumCoverId"));
    m_hash.insert(Qt::UserRole + 5, QByteArray("albumName"));
    m_hash.insert(Qt::UserRole + 6, QByteArray("albumCover"));
    m_hash.insert(Qt::UserRole + 7, QByteArray("trackName"));
    m_hash.insert(Qt::UserRole + 8, QByteArray("type"));
    m_hash.insert(Qt::UserRole + 9, QByteArray("duration"));
    m_hash.insert(Qt::UserRole + 10, QByteArray("storageDir"));
    m_hash.insert(Qt::UserRole + 11, QByteArray("liked"));
}

int SearchModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_playList.size();
}

inline void delayy(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

QVariant SearchModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_playList.size())
        return QVariant();

    TrackObject* item = m_playList.at(index.row());
    if (role == Qt::UserRole) {
        return item->trackId;
    } else if (role == Qt::UserRole + 1) {
        return item->artistId;
    } else if (role == Qt::UserRole + 2) {
        return item->artistName;
    } else if (role == Qt::UserRole + 3) {
        return item->artistCover;
    } else if (role == Qt::UserRole + 4) {
        return item->albumCoverId;
    } else if (role == Qt::UserRole + 5) {
        return item->albumName;
    } else if (role == Qt::UserRole + 6) {
        return item->albumCover;
    } else if (role == Qt::UserRole + 7) {
        return item->trackName;
    } else if (role == Qt::UserRole + 8) {
        return item->type;
    } else if (role == Qt::UserRole + 9) {
        return item->duration;
    } else if (role == Qt::UserRole + 10) {
        return item->storageDir;
    } else if (role == Qt::UserRole + 11) {
        return item->liked;
    }
    return QVariant();
}

bool SearchModel::insertRows(int position, int rows, TrackObject* item, const QModelIndex& index)
{
    Q_UNUSED(index);
    if (!(m_playList.contains(item))) {
        beginInsertRows(QModelIndex(), position, position + rows - 1);
        for (int row = 0; row < rows; ++row) {
            if (!(m_playList.contains(item))) {
                m_playList.insert(position, item);
            }
        }
        endInsertRows();
    }
    return true;
}

bool SearchModel::removeRows(int position, int rows, const QModelIndex& index)
{
    Q_UNUSED(index);
    if ((position + rows) > m_playList.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        m_playList.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void SearchModel::setCurrentIndex(int currentIndex)
{

    if (currentIndex >= 0 && currentIndex < m_playList.size() && currentIndex != m_currentIndex) {
        m_currentIndex = currentIndex;
        m_currentSong = m_playList.at(currentIndex)->trackName;
        m_currentArtist = m_playList.at(currentIndex)->artistName;
        emit currentIndexChanged(currentIndex);
    }
}

QVariant SearchModel::get(int idx)
{
    if (idx >= m_playList.size()) {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;

    TrackObject* item = m_playList.at(idx);

    itemData.insert("trackId", item->trackId);
    itemData.insert("artistId", item->artistId);
    itemData.insert("artistName", item->artistName);
    itemData.insert("artistCover", item->artistCover);
    itemData.insert("albumCoverId", item->albumCoverId);
    itemData.insert("albumName", item->albumName);
    itemData.insert("albumCover", item->albumCover);
    itemData.insert("trackName", item->trackName);
    itemData.insert("type", item->type);
    itemData.insert("duration", item->duration);
    itemData.insert("storageDir", item->storageDir);
    itemData.insert("liked", item->liked);

    return QVariant(itemData);
}

void SearchModel::playTrack()
{
    // https://api.music.yandex.net/play-audio?total-played-seconds=0.1&track-length-seconds=281.983&client-now=2022-04-24T06:05:30.742Z&album-id=5939666&end-position-seconds=0.1&from-cache=false&timestamp=2022-04-24T06:05:30.735Z&track-id=44317484&uid=253482261&from=radio-mobile-user-onyourwave-default&play-id=B0A28CEF-61ED-4373-B3A8-B46B545C6096&restored=true

    QUrlQuery query;
    QSettings settings;
    QDateTime current = QDateTime::currentDateTime();
    QString curdt = current.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    query.addQueryItem("client-now", curdt);
    query.addQueryItem("from-cache", "false");
    query.addQueryItem("track-id", QString::number(m_playList.at(m_currentIndex)->albumCoverId));
    query.addQueryItem("track-length-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    query.addQueryItem("end-position-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    query.addQueryItem("from", "mobile-home-rup_main-user-onyourwave-default");
    query.addQueryItem("track-id", QString::number(m_playList.at(m_currentIndex)->trackId));
    query.addQueryItem("play-id", "79CFB84C-4A0B-4B31-8954-3006C0BD9274");
    query.addQueryItem("timestamp", curdt);
    query.addQueryItem("total-played-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    qDebug() << query.toString();
    Request playTrackRequest("/play-audio?" + query.toString());
    playTrackRequest.post();
}

void SearchModel::sendFeedback(QString type)
{
    QDateTime current = QDateTime::currentDateTime();
    QString curdt = current.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    QJsonObject o1;
    if (type.contains("trackStarted")) {
        o1 = {
            { "type", type },
            { "timestamp", curdt },
            { "totalPlayedSeconds", 0 },
            { "trackId", QString::number(m_playList.at(m_currentIndex)->trackId) + ":" + QString::number(m_playList.at(m_currentIndex)->albumCoverId) }
        };
    } else {
        o1 = {
            { "type", type },
            { "timestamp", curdt },
            { "totalPlayedSeconds", QString::number(m_playList.at(m_currentIndex)->duration) },
            { "trackId", QString::number(m_playList.at(m_currentIndex)->trackId) + ":" + QString::number(m_playList.at(m_currentIndex)->albumCoverId) }
        };
    }
    QString strFromObj = QJsonDocument(o1).toJson(QJsonDocument::Compact).toStdString().c_str();
    Request* sendFeedbackRequest = new Request("/rotor/station/user:onyourwave/feedback?batch-id=" + batchid);
    sendFeedbackRequest->post(strFromObj);
}

void SearchModel::searchTracks(QString q)
{
    if (m_loading) {
        return;
    }
    m_loading = true;

    beginRemoveRows(QModelIndex(), 0, m_playList.size() - 1);

    m_playList.clear();

    endRemoveRows();

    QUrlQuery query;

    query.addQueryItem("inputType", "0");
    query.addQueryItem("page", "0");
    query.addQueryItem("text", q);
    query.addQueryItem("from", "suggest");
    query.addQueryItem("type", "track");
    query.addQueryItem("nocorrect", "false");

    Request* serchRequest = new Request("/search");
    serchRequest->get(query);
    connect(serchRequest, &Request::dataReady, this, &SearchModel::getSearchTracksFinished);
}

QList<TrackObject*> SearchModel::playlist()
{

    return m_playList;
}

void SearchModel::getSearchTracksFinished(const QJsonValue& value)
{
    if (value == m_oldValue) {
        /*Sometimes Yandex return data twice*/
        return;
    } else {
        m_oldValue = value;
    }

    QJsonObject qjo = value.toObject();
    QJsonObject tracks = qjo["tracks"].toObject();
    QJsonArray results = tracks["results"].toArray();

    foreach (const QJsonValue& value, results) {
        QJsonObject trackObject = value.toObject();
        TrackObject* newTrack = new TrackObject;
        newTrack->trackId = trackObject["id"].toInt();
        newTrack->artistId = trackObject["artists"].toArray().at(0).toObject()["id"].toInt();
        newTrack->artistName = trackObject["artists"].toArray().at(0).toObject()["name"].toString();
        newTrack->artistCover = trackObject["artists"].toArray().at(0).toObject()["cover"].toObject()["uri"].toString();
        newTrack->albumCoverId = trackObject["albums"].toArray().at(0).toObject()["id"].toInt();
        qDebug() << "albumId: " << QString::number(newTrack->albumCoverId);
        newTrack->albumName = trackObject["albums"].toArray().at(0).toObject()["title"].toString();
        newTrack->albumCover = trackObject["albums"].toArray().at(0).toObject()["coverUri"].toString();
        newTrack->trackName = trackObject["title"].toString();
        newTrack->type = trackObject["type"].toString();
        newTrack->duration = trackObject["durationMs"].toInt();
        newTrack->storageDir = "";
        newTrack->liked = false;

        if (m_playList.size() == 0) {
            emit loadFirstDataFinished();
        }

        beginInsertRows(QModelIndex(), m_playList.size(), m_playList.size());

        // DOT USE CACHER IN MODEL!

        /*Cacher* cacher = new Cacher(newTrack);
        cacher->saveToCache();
        newTrack->fileUrl = cacher->fileToSave();
        newTrack->url = cacher->Url();*/
        m_playList.push_back(newTrack);
        endInsertRows();
    }

    m_loading = false;
}
