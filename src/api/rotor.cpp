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

void Rotor::getStationTracks(QString stationId, QString lastTrackid)
{
    QUrlQuery query;
    query.addQueryItem("settings2", "true");

    if (lastTrackid.toInt() > 0) {
        query.addQueryItem("queue", lastTrackid);
    }

    Request* getStationTracksRequest = new Request("/rotor/station/" + stationId + "/tracks");
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

void Rotor::postStantionFeedback(QString stationId)
{
    Q_UNUSED(stationId)
}

void Rotor::getStationInfoRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}

void Rotor::getStationTracksRequestHandler(QJsonValue value)
{
    QJsonArray tracks = value.toObject()["sequence"].toArray();
    foreach (const QJsonValue& value, tracks) {
        QJsonObject trackObject = value.toObject().value("track").toObject();
        Track* trackToAdd = new Track(trackObject);
        if (trackToAdd->trackId() != 0) {
            stantionTrackReady(trackToAdd);
        }
    }
}

void Rotor::getAccountStatusRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}

void Rotor::getStantionsListRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}

void Rotor::getStantionsDashboardRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}

void Rotor::getStantionFeedbackRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}
