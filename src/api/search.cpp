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

#include "search.h"
#include "request.h"

#include <QJsonArray>

Search::Search(QObject *parent)
    : QObject(parent)
    , m_query("")
    , m_perPage(20)
    , m_page(0)
    , m_type(SearchType::All)
{
    connect(this, &Search::queryChanged, this, &Search::makeQuery);
    connect(this, &Search::perPageChanged, this, &Search::makeQuery);
    connect(this, &Search::pageChanged, this, &Search::makeQuery);
    connect(this, &Search::typeChanged, this, &Search::makeQuery);
}

const QString &Search::query() const
{
    return m_query;
}

void Search::setQuery(const QString &newQuery)
{
    if (m_query == newQuery.trimmed())
        return;
    m_query = newQuery;
    emit queryChanged();
}

int Search::perPage() const
{
    return m_perPage;
}

void Search::setPerPage(int newPerPage)
{
    if (m_perPage == newPerPage)
        return;
    m_perPage = newPerPage;
    emit perPageChanged();
}

int Search::page() const
{
    return m_page;
}

void Search::setPage(int newPage)
{
    if (m_page == newPage)
        return;
    m_page = newPage;
    emit pageChanged();
}

const Search::SearchType &Search::type() const
{
    return m_type;
}

void Search::setType(const SearchType &newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

void Search::makeQuery()
{
    if(m_query.length() < 3) {
        return;
    }

    emit searchStarted();

    Request* searchRequest = new Request("/search");
    connect(searchRequest, &Request::dataReady, this, &Search::searchRequestHandler);

    QUrlQuery query;
    query.addQueryItem("text", m_query);
    query.addQueryItem("page", QString::number(m_page));

    QString typeString;
    switch (m_type) {
    case Search::SearchType::AlbumsSearch:
        typeString = "album";
        break;
    case Search::SearchType::ArtistsSearch:
        typeString = "artist";
        break;
    case Search::SearchType::PodcastsSearch:
        typeString = "pogcast";
        break;
    default:
        typeString = "all";
    }
    query.addQueryItem("type", typeString);

    searchRequest->get(query);
}

void Search::searchRequestHandler(QJsonValue value)
{
    SearchResultData* searchResult = new SearchResultData();

    //Artists
    QJsonArray artistsResult = value.toObject()["artists"].toObject().value("results").toArray();
    int artistConut = artistsResult.count();
    if(artistsResult.count() > 3) {
        artistConut = 3;
    }

    for(int i=0; i < artistConut; i++) {
        Artist* artist = new Artist(artistsResult[i].toObject());
        if(!artist->coverUri().isEmpty()) {
            searchResult->artists.push_back(artist);
        }
    }

    //Albums
    QJsonArray albumsResult = value.toObject()["albums"].toObject().value("results").toArray();
    int albumsConut = albumsResult.count();
    if(albumsResult.count() > 3) {
        albumsConut = 3;
    }

    for(int i=0; i < albumsConut; i++) {
        Album* album = new Album(albumsResult[i].toObject());
        if(!album->coverUri().isEmpty()) {
            searchResult->albums.push_back(album);
        }
    }

    //tracks
    QJsonArray tracksResult = value.toObject()["tracks"].toObject().value("results").toArray();
    int tracksConut = albumsResult.count();
    if(albumsResult.count() > 3) {
        albumsConut = 3;
    }

    for(int i=0; i < tracksConut; i++) {
        Track* track = new Track(albumsResult[i].toObject());
        if(!track->trackId().isEmpty()) {
            searchResult->tracks.push_back(track);
        }
    }

    emit searchReady(searchResult);
}
