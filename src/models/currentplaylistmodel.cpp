/*
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
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
{
    connect(this, &QAbstractListModel::rowsInserted, this, &CurrentPlayListModel::rowCountChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &CurrentPlayListModel::rowCountChanged);
}

int CurrentPlayListModel::currentIndex() const
{
    return m_currentIndex;
}

void CurrentPlayListModel::setCurrentIndex(int newCurrentIndex)
{
    if (m_currentIndex == newCurrentIndex)
        return;
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
    if (!track->trackId().isEmpty()) {
        beginInsertRows(QModelIndex(), m_currentTracks.count(), m_currentTracks.count());
        m_currentTracks.push_back(track);
        endInsertRows();
        emit rowCountChanged();
    } else {
        qWarning() << "Wrong track";
    }
}

void CurrentPlayListModel::setPlaylist(Playlist *playlist)
{
    if(playlist == nullptr) {
        return;
    }
    clear();
    beginInsertRows(QModelIndex(), 0, playlist->tracks().count());
    for(QObject* trackObject: playlist->tracks()) {
        Track* track = reinterpret_cast<Track*>(trackObject);
        if(track != nullptr) {
            m_currentTracks.push_back(track);
        }
    }
    endInsertRows();
}

Track* CurrentPlayListModel::getTrack(int index)
{
    if (index < m_currentTracks.count()) {
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

void CurrentPlayListModel::clear()
{
    beginResetModel();
    m_currentTracks.clear();
    m_currentIndex = -1;
    endResetModel();
    currentIndexChanged();
}
