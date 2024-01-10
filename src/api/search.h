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

#ifndef SEARCH_H
#define SEARCH_H

#include <QJsonValue>
#include <QObject>

#include "../types/album.h"
#include "../types/artist.h"
#include "../types/playlist.h"
#include "../types/track.h"
#include "../types/SearchResultData.h"

class Search : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(int perPage READ perPage WRITE setPerPage NOTIFY perPageChanged)
    Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(SearchType type READ type WRITE setType NOTIFY typeChanged)

public:
    enum SearchType {
        All = 0,
        ArtistsSearch,
        AlbumsSearch,
        TracksSearch,
        PodcastsSearch
    };
    Q_ENUMS(SearchType)

    explicit Search(QObject* parent = nullptr);

    const QString& query() const;
    void setQuery(const QString& newQuery);

    int perPage() const;
    void setPerPage(int newPerPage);

    int page() const;
    void setPage(int newPage);

    const SearchType& type() const;
    void setType(const SearchType& newType);

signals:
    void queryChanged();
    void perPageChanged();
    void pageChanged();
    void typeChanged();

    void searchStarted();
    void searchReady(SearchResultData* result);

private slots:
    void makeQuery();
    void searchRequestHandler(QJsonValue value);

private:
    QString m_query;
    int m_perPage;
    int m_page;
    SearchType m_type;
};

#endif // SEARCH_H
