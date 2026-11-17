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

#include "rotor.h"
#include "request.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

Rotor::Rotor(QObject* parent)
    : QObject(parent)
{
}

QString Rotor::feedbackTypeToString(FeedbackType type)
{
    switch (type) {
    case FeedbackType::RadioStarted:
        return QStringLiteral("radioStarted");
    case FeedbackType::TrackFinished:
        return QStringLiteral("trackFinished");
    case FeedbackType::TrackStarted:
        return QStringLiteral("trackStarted");
    case FeedbackType::Skip:
        return QStringLiteral("skip");
    }
    return QString();
}

QString Rotor::trackFullId(Track* track) const
{
    if (!track || track->trackId() <= 0) {
        return QString();
    }
    QString id = QString::number(track->trackId());
    if (!track->albums().isEmpty()) {
        QObject *obj = track->albums().first();
        Album* album = qobject_cast<Album*>(obj);
        if (!album) {
            album = reinterpret_cast<Album*>(obj);
        }
        if (album && album->albumId() > 0) {
            id += ":" + QString::number(album->albumId());
        }
    }
    return id;
}

void Rotor::getStationInfo(const QString &stationId)
{
    m_lastStationId = stationId;
    Request* request = new Request("/rotor/station/" + stationId + "/info");
    connect(request, &Request::dataReady, this, &Rotor::getStationInfoRequestHandler);
    request->get();
}

void Rotor::getStationTracks(const QString &stationId, int lastTrackid)
{
    QString queue;
    if (lastTrackid >= 0) {
        queue = QString::number(lastTrackid);
    }
    getStationTracks(stationId, queue, true);
}

void Rotor::getStationTracks(const QString &stationId, const QString &queueTrackId, bool settings2)
{
    m_lastStationId = stationId;
    QUrlQuery query;
    if (settings2) {
        query.addQueryItem(QStringLiteral("settings2"), QStringLiteral("true"));
    }
    if (!queueTrackId.isEmpty() && queueTrackId != "-1") {
        query.addQueryItem(QStringLiteral("queue"), queueTrackId);
    }

    Request* request = new Request("/rotor/station/" + stationId + "/tracks");
    connect(request, &Request::dataReady, this, &Rotor::getStationTracksRequestHandler);
    request->get(query);
}

void Rotor::getAccountStatus()
{
    Request* request = new Request("/rotor/account/status");
    connect(request, &Request::dataReady, this, &Rotor::getAccountStatusRequestHandler);
    request->get();
}

void Rotor::getStationsList(const QString &language)
{
    QUrlQuery query;
    if (!language.isEmpty()) {
        query.addQueryItem(QStringLiteral("language"), language);
    }

    Request* request = new Request("/rotor/stations/list");
    connect(request, &Request::dataReady, this, &Rotor::getStantionsListRequestHandler);
    if (query.isEmpty()) {
        request->get();
    } else {
        request->get(query);
    }
}

void Rotor::getStantionsList()
{
    getStationsList();
}

void Rotor::getStationsDashboard()
{
    Request* request = new Request("/rotor/stations/dashboard");
    connect(request, &Request::dataReady, this, &Rotor::getStantionsDashboardRequestHandler);
    request->get();
}

void Rotor::getStantionsDashboard()
{
    getStationsDashboard();
}

void Rotor::postStationFeedback(FeedbackType type,
                                const QString &stationId,
                                const QString &trackId,
                                int totalPlayedSeconds,
                                const QString &from,
                                const QString &batchId)
{
    const QString typeString = feedbackTypeToString(type);
    m_lastFeedbackType = typeString;

    QJsonObject body;
    body.insert(QStringLiteral("type"), typeString);
    body.insert(QStringLiteral("timestamp"), QDateTime::currentDateTimeUtc().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss.zzzZ")));
    body.insert(QStringLiteral("from"), from.isEmpty() ? QStringLiteral("mobile-radio-user-onyourwave") : from);

    if (!trackId.isEmpty()) {
        body.insert(QStringLiteral("trackId"), trackId);
    }
    if (type == FeedbackType::TrackFinished || type == FeedbackType::Skip) {
        body.insert(QStringLiteral("totalPlayedSeconds"), QString::number(qMax(0, totalPlayedSeconds)));
    } else {
        body.insert(QStringLiteral("totalPlayedSeconds"), 0);
    }

    // Spec: batch-id must be absent for radioStarted
    QString path = "/rotor/station/" + stationId + "/feedback";
    const QString effectiveBatch = batchId.isEmpty() ? m_batchId : batchId;
    if (type != FeedbackType::RadioStarted && !effectiveBatch.isEmpty()) {
        QUrlQuery q;
        q.addQueryItem(QStringLiteral("batch-id"), effectiveBatch);
        path += "?" + q.toString(QUrl::FullyEncoded);
    }

    Request* request = new Request(path);
    connect(request, &Request::dataReady, this, &Rotor::postStantionFeedbackRequestHandler);
    request->post(QJsonDocument(body).toJson(QJsonDocument::Compact));
}

void Rotor::postStationFeedback(FeedbackType type, Track* track, const QString &stationId, int totalPlayedSeconds)
{
    const QString tId = trackFullId(track);
    postStationFeedback(type, stationId, tId, totalPlayedSeconds);
}

void Rotor::postStantionFeedback(FeedbackType type, Track* track, const QString &stationId, int totalPlayedSeconds)
{
    postStationFeedback(type, track, stationId, totalPlayedSeconds);
}

void Rotor::getStationInfoRequestHandler(QJsonValue value)
{
    emit stationInfoReady(m_lastStationId, value);
}

void Rotor::getStationTracksRequestHandler(QJsonValue value)
{
    QList<QObject*> tracksList;
    QJsonObject obj = value.toObject();
    m_batchId = obj.value(QStringLiteral("batchId")).toString();
    const QJsonArray sequence = obj.value(QStringLiteral("sequence")).toArray();
    for (const QJsonValue& v : sequence) {
        QJsonObject item = v.toObject();
        QJsonObject trackObject = item.value(QStringLiteral("track")).toObject();
        if (trackObject.isEmpty()) {
            continue;
        }
        Track* trackToAdd = new Track(trackObject);
        if (trackToAdd && trackToAdd->trackId() != 0) {
            tracksList.push_back(trackToAdd);
        } else {
            delete trackToAdd;
        }
    }

    emit stationTracksReady(m_lastStationId, tracksList, m_batchId, obj);
    emit stantionTracksReady(tracksList); // compatibility
}

void Rotor::getAccountStatusRequestHandler(QJsonValue value)
{
    emit accountStatusReady(value);
}

void Rotor::getStantionsListRequestHandler(QJsonValue value)
{
    QJsonArray arr;
    if (value.isArray()) {
        arr = value.toArray();
    } else if (value.isObject() && value.toObject().contains(QStringLiteral("stations"))) {
        arr = value.toObject().value(QStringLiteral("stations")).toArray();
    }
    emit stationsListReady(arr);
}

void Rotor::getStantionsDashboardRequestHandler(QJsonValue value)
{
    emit stationsDashboardReady(value);
}

void Rotor::postStantionFeedbackRequestHandler(QJsonValue value)
{
    emit stationFeedbackSent(m_lastFeedbackType, value);
    if (value.isString() && value.toString() != QStringLiteral("ok")) {
        qWarning() << Q_FUNC_INFO << value;
    }
}

