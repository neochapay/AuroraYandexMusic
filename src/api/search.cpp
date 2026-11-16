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

#include "search.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

Search::Search(QObject *parent)
    : QObject(parent)
    , m_query("")
    , m_perPage(20)
    , m_page(0)
    , m_type(SearchType::All)
    , m_noCorrect(false)
{
    connect(this, &Search::queryChanged, this, &Search::makeQuery);
    connect(this, &Search::perPageChanged, this, &Search::makeQuery);
    connect(this, &Search::pageChanged, this, &Search::makeQuery);
    connect(this, &Search::typeChanged, this, &Search::makeQuery);
    connect(this, &Search::noCorrectChanged, this, &Search::makeQuery);
}

const QString &Search::query() const
{
    return m_query;
}

void Search::setQuery(const QString &newQuery)
{
    if (m_query == newQuery.trimmed())
        return;
    m_query = newQuery.trimmed();
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

Search::SearchType Search::type() const
{
    return m_type;
}

void Search::setType(SearchType newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

bool Search::noCorrect() const
{
    return m_noCorrect;
}

void Search::setNoCorrect(bool noCorrect)
{
    if (m_noCorrect == noCorrect)
        return;
    m_noCorrect = noCorrect;
    emit noCorrectChanged();
}

QString Search::typeToString(SearchType type)
{
    switch (type) {
    case Search::SearchType::AlbumsSearch:
        return QStringLiteral("album");
    case Search::SearchType::ArtistsSearch:
        return QStringLiteral("artist");
    case Search::SearchType::TracksSearch:
        return QStringLiteral("track");
    case Search::SearchType::PodcastsSearch:
        return QStringLiteral("podcast");
    case Search::SearchType::All:
    default:
        return QStringLiteral("all");
    }
}

void Search::search(const QString &text, int page)
{
    if (!text.isEmpty()) {
        m_query = text.trimmed();
        emit queryChanged();
        // makeQuery triggered by signal; if page also changed below, second query will follow but acceptable
    }
    if (page >= 0 && page != m_page) {
        m_page = page;
        emit pageChanged();
        return;
    }
    if (!text.isEmpty()) {
        return; // queryChanged triggered
    }
    makeQuery();
}

void Search::suggest(const QString &part)
{
    if (part.trimmed().isEmpty()) {
        return;
    }
    Request* request = new Request("/search/suggest");
    connect(request, &Request::dataReady, this, &Search::suggestRequestHandler);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("part"), part.trimmed());
    request->get(query);
}

void Search::makeQuery()
{
    if (m_query.length() < 3) {
        return;
    }

    emit searchStarted();

    Request* searchRequest = new Request("/search");
    connect(searchRequest, &Request::dataReady, this, &Search::searchRequestHandler);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("text"), m_query);
    query.addQueryItem(QStringLiteral("page"), QString::number(m_page));
    query.addQueryItem(QStringLiteral("type"), typeToString(m_type));
    // Spec parameter name is "nococrrect" (typo preserved in some clients/docs)
    query.addQueryItem(QStringLiteral("nococrrect"), m_noCorrect ? QStringLiteral("true") : QStringLiteral("false"));

    searchRequest->get(query);
}

void Search::searchRequestHandler(QJsonValue value)
{
    QJsonObject obj = value.toObject();
    emit searchResultReady(obj);

    SearchResultData* searchResult = new SearchResultData();

    // Artists
    const QJsonArray artistsResult = obj.value("artists").toObject().value("results").toArray();
    for (const QJsonValue &v : artistsResult) {
        Artist* artist = new Artist(v.toObject());
        if (artist) {
            searchResult->artists.push_back(artist);
        }
    }

    // Albums
    const QJsonArray albumsResult = obj.value("albums").toObject().value("results").toArray();
    for (const QJsonValue &v : albumsResult) {
        Album* album = new Album(v.toObject());
        if (album && (album->albumId() != 0 || !album->title().isEmpty())) {
            searchResult->albums.push_back(album);
        }
    }

    // Tracks
    const QJsonArray tracksResult = obj.value("tracks").toObject().value("results").toArray();
    for (const QJsonValue &v : tracksResult) {
        Track* track = new Track(v.toObject());
        if (track && track->trackId() != 0) {
            searchResult->tracks.push_back(track);
        } else {
            delete track;
        }
    }

    emit searchReady(searchResult);
}

void Search::suggestRequestHandler(QJsonValue value)
{
    QJsonObject obj = value.toObject();
    emit suggestReady(obj);
}

