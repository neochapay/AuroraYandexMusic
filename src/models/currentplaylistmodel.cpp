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
#include <QDebug>

#include "currentplaylistmodel.h"

CurrentPlayListModel::CurrentPlayListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_currentIndex(-1)
    , m_prevTrack(nullptr)
    , m_nextTrack(nullptr)
{
    connect(this, &QAbstractListModel::rowsInserted, this, &CurrentPlayListModel::rowCountChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &CurrentPlayListModel::rowCountChanged);

    connect(this, &CurrentPlayListModel::currentIndexChanged, this, &CurrentPlayListModel::calculateNextready);
}

int CurrentPlayListModel::currentIndex() const
{
    return m_currentIndex;
}

void CurrentPlayListModel::setCurrentIndex(int newCurrentIndex)
{
    if (m_currentIndex == newCurrentIndex)
        return;
    m_prevTrack = getTrack(m_currentIndex);

    m_currentIndex = newCurrentIndex;
    emit currentIndexChanged();
}

QVariant CurrentPlayListModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

int CurrentPlayListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_currentTracks.count();
}

bool CurrentPlayListModel::insertRow(int position, Track* track, const QModelIndex& index)
{
    Q_UNUSED(index);
    if(track == nullptr) {
        return false;
    }

    if (!(m_currentTracks.contains(track))) {
        beginInsertRows(QModelIndex(), position, position);
        if (!(m_currentTracks.contains(track))) {
            m_currentTracks.insert(position, track);
        }
        endInsertRows();
        return true;
    }
    return false;
}

bool CurrentPlayListModel::removeRows(int position, int rows, const QModelIndex& index)
{
    Q_UNUSED(index);
    if ((position + rows) > m_currentTracks.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        m_currentTracks.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void CurrentPlayListModel::push(Track* track)
{
    if(track == nullptr) {
        qWarning() << "Wrong track";
        return;
    }

    if (track->trackId() != 0) {
        beginInsertRows(QModelIndex(), m_currentTracks.count(), m_currentTracks.count());
        m_currentTracks.push_back(track);
        endInsertRows();
        emit rowCountChanged();
        updateDownloadQueue();
    } else {
        qWarning() << "Wrong track";
    }
}

void CurrentPlayListModel::setPlaylist(Playlist *playlist)
{
    if(playlist == nullptr) {
        return;
    }
    cancelAllDownloads();
    clear();
    beginInsertRows(QModelIndex(), 0, playlist->tracks().count());
    for(QObject* trackObject: playlist->tracks()) {
        Track* track = reinterpret_cast<Track*>(trackObject);
        if(track != nullptr) {
            m_currentTracks.push_back(track);
        }
    }
    endInsertRows();
    emit rowCountChanged();
    updateDownloadQueue();
}

Track* CurrentPlayListModel::getTrack(int index)
{
    if (index >= 0 && index < m_currentTracks.count()) {
        return m_currentTracks.at(index);
    }
    return nullptr;
}

Track* CurrentPlayListModel::getCurrentTrack()
{
    if (m_currentIndex != -1 && m_currentIndex < m_currentTracks.count()) {
        return m_currentTracks.at(m_currentIndex);
    }
    return nullptr;
}

Track *CurrentPlayListModel::getPrevTrack()
{
    return m_prevTrack;
}

void CurrentPlayListModel::clear()
{
    cancelAllDownloads();
    beginResetModel();
    m_currentTracks.clear();
    m_currentIndex = -1;
    m_nextTrack = nullptr;
    endResetModel();
    emit rowCountChanged();
    emit currentIndexChanged();
}

void CurrentPlayListModel::cancelAllDownloads()
{
    for (Track* track : m_currentTracks) {
        if (track) {
            track->cancelDownload();
        }
    }
    m_nextTrack = nullptr;
}

void CurrentPlayListModel::updateDownloadQueue()
{
    if (m_currentIndex < 0 || m_currentTracks.isEmpty()) {
        return;
    }

    for (Track* track : m_currentTracks) {
        if (track) {
            connect(track, &Track::trackChanged, this, [=] {
                emit nextReadyChanged();
                updateDownloadQueue();
            }, Qt::UniqueConnection);
            connect(track, &Track::downloadProgressChanged, this, [=] {
                if (!track->downloading()) {
                    emit nextReadyChanged();
                    updateDownloadQueue();
                }
            }, Qt::UniqueConnection);
        }
    }

    for (Track* track : m_currentTracks) {
        if (track && track->downloading()) {
            return;
        }
    }

    for (int i = m_currentIndex; i < m_currentTracks.count(); ++i) {
        Track* track = m_currentTracks.at(i);
        if (track == nullptr || track->downloaded()) {
            continue;
        }
        track->download();
        return;
    }
}

bool CurrentPlayListModel::nextReady() const
{
    if (m_currentIndex < 0 || m_currentIndex >= m_currentTracks.count() - 1) {
        return false;
    }

    Track* nextTrack = m_currentTracks.at(m_currentIndex + 1);
    return nextTrack != nullptr && nextTrack->downloaded();
}

void CurrentPlayListModel::calculateNextready()
{
    m_nextTrack = getTrack(m_currentIndex + 1);
    if (m_nextTrack != nullptr) {
        connect(m_nextTrack, &Track::trackChanged, this, [=] {
            emit nextReadyChanged();
            updateDownloadQueue();
        }, Qt::UniqueConnection);
        connect(m_nextTrack, &Track::downloadProgressChanged, this, [=] {
            emit nextReadyChanged();
        }, Qt::UniqueConnection);
    }

    Track* currentTrack = getCurrentTrack();
    if (currentTrack != nullptr) {
        connect(currentTrack, &Track::trackChanged, this, [=] {
            emit nextReadyChanged();
            updateDownloadQueue();
        }, Qt::UniqueConnection);
    }

    emit nextReadyChanged();
    updateDownloadQueue();
}

