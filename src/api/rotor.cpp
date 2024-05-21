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

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

Rotor::Rotor(QObject* parent)
    : QObject(parent)
{
}

void Rotor::getStationInfo(QString stationId)
{
    Request* getStationInfoRequest = new Request("/rotor/station/" + stationId + "/info");
    connect(getStationInfoRequest, &Request::dataReady, this, &Rotor::getStationInfoRequestHandler);

    getStationInfoRequest->get();
}

void Rotor::getStationTracks(QString stationId, int lastTrackid)
{
    QUrlQuery query;
    query.addQueryItem("settings2", "true");

    if (lastTrackid >= 0) {
        query.addQueryItem("queue", QString::number(lastTrackid));
    }

    Request* getStationTracksRequest = new Request("/rotor/station/" + stationId + "/tracks");
    getStationTracksRequest->setDebug(true);
    connect(getStationTracksRequest, &Request::dataReady, this, &Rotor::getStationTracksRequestHandler);
    getStationTracksRequest->get(query);
}

void Rotor::getAccountStatus()
{
    Request* getAccountStatusRequest = new Request("/rotor/account/status");
    connect(getAccountStatusRequest, &Request::dataReady, this, &Rotor::getAccountStatusRequestHandler);

    getAccountStatusRequest->get();
}

void Rotor::getStantionsList()
{
    Request* getStantionsListRequest = new Request("/rotor/stations/list");
    connect(getStantionsListRequest, &Request::dataReady, this, &Rotor::getStantionsListRequestHandler);

    getStantionsListRequest->get();
}

void Rotor::getStantionsDashboard()
{
    Request* getStantionsDashboardRequest = new Request("/rotor/stations/dashboard");
    connect(getStantionsDashboardRequest, &Request::dataReady, this, &Rotor::getStantionsDashboardRequestHandler);

    getStantionsDashboardRequest->get();
}

void Rotor::postStantionFeedback(FeedbackType type, Track* track, QString stationId, int totalPlayedSeconds)
{
    if(track == nullptr || track->trackId() == -1) {
        return;
    }

    QString typeSting;
    switch (type) {
    case FeedbackType::RadioStarted:
        typeSting = "radioStarted";
        break;
    case FeedbackType::TrackFinished:
        typeSting = "trackFinished";
        break;
    case FeedbackType::TrackStarted:
        typeSting = "trackStarted";
        break;
    case FeedbackType::Skip:
        typeSting = "skip";
        break;
    }

    QString from = "mobile-radio-user-onyourwave";
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    Album* album = reinterpret_cast<Album*>(track->albums().first());
    QString trackId = QString::number(track->trackId()) + ":" + QString::number(album->albumId());

    QJsonObject query;
    query.insert("type", typeSting);
    query.insert("timestamp", timestamp);
    if(type == FeedbackType::TrackFinished || type == FeedbackType::Skip) {
        query.insert("totalPlayedSeconds", QString::number(totalPlayedSeconds));
    } else {
        query.insert("totalPlayedSeconds", 0);
    }
    query.insert("trackId", trackId);
    query.insert("from", from);
    QString batchApx;
    if(type == FeedbackType::RadioStarted && !m_batchId.isEmpty()) {
        batchApx = "?batch-id="+m_batchId;
    }

    Request* postStantionFeedbackRequest = new Request("/rotor/station/"+stationId+"/feedback"+batchApx);
    connect(postStantionFeedbackRequest, &Request::dataReady, this, &Rotor::postStantionFeedbackRequestHandler);
    postStantionFeedbackRequest->post(QJsonDocument(query).toJson(QJsonDocument::Compact));
}

void Rotor::getStationInfoRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::getStationTracksRequestHandler(QJsonValue value)
{   
    QList<QObject*> tracksList;
    m_batchId = value.toObject().value("batchId").toString();
    QJsonArray tracks = value.toObject().value("sequence").toArray();
    foreach (const QJsonValue& value, tracks) {
        QJsonObject trackObject = value.toObject().value("track").toObject();
        Track* trackToAdd = new Track(trackObject);
        if (trackToAdd->trackId() != 0) {
            tracksList.push_back(trackToAdd);
        }
    }
    emit stantionTracksReady(tracksList);

    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::getAccountStatusRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::getStantionsListRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::getStantionsDashboardRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::getStantionFeedbackRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void Rotor::postStantionFeedbackRequestHandler(QJsonValue value)
{
    if(value.toString() != "ok") {
        qWarning() << Q_FUNC_INFO << value;
    }
    Request* request = qobject_cast<Request*>(sender());
    delete request;
}
