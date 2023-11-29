#include "../YaSailMusic.h"
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
#include <QStandardPaths>
#include <QThread>
#include <QTimer>
#include <QUrlQuery>
#include <QtQml>

#include "../api/request.h"
#include "../authorization.h"
#include "../cacher.h"
#include "../settings.h"
#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject* parent)
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

    baseValues_->currentPlaylist = m_playList;

    connect(this, &QAbstractListModel::rowsInserted, this, &PlaylistModel::rowCountChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &PlaylistModel::rowCountChanged);
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_playList.size();
}

void PlaylistModel::setNewData()
{
    m_playList = baseValues_->currentPlaylist;
}

inline void delayy(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
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

bool PlaylistModel::insertRows(int position, int rows, TrackObject* item, const QModelIndex& index)
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

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex& index)
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

void PlaylistModel::setCurrentIndex(int currentIndex)
{
    if (currentIndex >= 0 && currentIndex < m_playList.size() && currentIndex != m_currentIndex) {
        m_currentIndex = currentIndex;
        m_currentSong = m_playList.at(currentIndex)->trackName;
        m_currentArtist = m_playList.at(currentIndex)->artistName;
        emit currentIndexChanged(currentIndex);

        if (m_currentIndex == m_playList.size() - 1) {
            qDebug() << "Load new tracks!";
            loadMyWave();
        }
    }
}

QVariant PlaylistModel::get(int idx)
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

void PlaylistModel::playTrack()
{
    QUrlQuery query;
    Settings settings;
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
    Request* playTrackRequest = new Request("/play-audio?" + query.toString());
    playTrackRequest->post(QString(""));
}

void PlaylistModel::sendFeedback(QString type)
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
    qDebug() << "JSON: " << strFromObj;
    Request* sendFeedbackRequest = new Request("/rotor/station/user:onyourwave/feedback?batch-id=" + batchid);
    sendFeedbackRequest->post(strFromObj);
}

void PlaylistModel::loadMyWave()
{
    if (m_loading) {
        return;
    }
    m_loading = true;

    QUrlQuery query;
    query.addQueryItem("settings2", "true");
    if (m_playList.size() > 0) {
        query.addQueryItem("queue", QString::number(m_playList.at(m_playList.size() - 1)->trackId));
    }
    Request* loadWaveRequest = new Request("/rotor/station/user:onyourwave/tracks");
    loadWaveRequest->get(query);
    connect(loadWaveRequest, &Request::dataReady, this, &PlaylistModel::getWaveFinished);
}

void PlaylistModel::getWaveFinished(const QJsonValue& value)
{
    if (value == m_oldValue) {
        /*Sometimes Yandex return data twice*/
        return;
    } else {
        m_oldValue = value;
    }

    QJsonObject qjo = value.toObject();
    QJsonArray tracks = qjo["sequence"].toArray();
    batchid = qjo["batchId"].toString();
    // beginInsertRows(QModelIndex(), m_playList.count(), m_playList.count()+tracks.count()-1);

    foreach (const QJsonValue& value, tracks) {
        QJsonObject trackObject = value.toObject();
        TrackObject* newTrack = new TrackObject;
        newTrack->trackId = trackObject["track"].toObject()["id"].toString().toInt();
        newTrack->artistId = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["id"].toInt();
        newTrack->artistName = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["name"].toString();
        newTrack->artistCover = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["cover"].toObject()["uri"].toString();
        newTrack->albumCoverId = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["id"].toInt();
        qDebug() << "albumId: " << QString::number(newTrack->albumCoverId);
        newTrack->albumName = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["title"].toString();
        newTrack->albumCover = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["coverUri"].toString();
        newTrack->trackName = trackObject["track"].toObject()["title"].toString();
        newTrack->type = trackObject["track"].toObject()["type"].toString();
        newTrack->duration = trackObject["track"].toObject()["durationMs"].toString().toInt();
        newTrack->storageDir = trackObject["track"].toObject()["storageDir"].toString();
        newTrack->liked = trackObject["liked"].toBool();

        if (m_playList.size() == 0) {
            emit loadFirstDataFinished();
        }

        if (!newTrack->albumName.isEmpty()
            && (!(m_playList.contains(newTrack)))
            && !newTrack->trackName.isEmpty()
            && (!(m_oldValue.toString().contains(trackObject["track"].toObject()["id"].toString())))) {

            beginInsertRows(QModelIndex(), m_playList.size(), m_playList.size());
            m_playList.push_back(newTrack);
            endInsertRows();
        }
    }

    m_loading = false;
    baseValues_->currentPlaylist = m_playList;
}
