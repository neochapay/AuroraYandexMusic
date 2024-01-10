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

#ifndef ROTOR_H
#define ROTOR_H

#include "../types/track.h"

#include <QJsonValue>
#include <QObject>

class Rotor : public QObject {
    Q_OBJECT

public:
    enum FeedbackType{
        RadioStarted,
        TrackStarted,
        TrackFinished,
        Skip
    };
    Q_ENUM(FeedbackType)

    explicit Rotor(QObject* parent = nullptr);
    Q_INVOKABLE void getStationInfo(QString stationId = "user:onyourwave");
    Q_INVOKABLE void getStationTracks(QString stationId = "user:onyourwave", QString lastTrackid = "0");
    Q_INVOKABLE void getAccountStatus();
    Q_INVOKABLE void getStantionsList();
    Q_INVOKABLE void getStantionsDashboard();
    Q_INVOKABLE void postStantionFeedback(FeedbackType type, Track* track, QString stationId = "user:onyourwave", int totalPlayedSeconds =0);

signals:
    void stantionTracksReady(QList<QObject*> tracks);

private slots:
    void getStationInfoRequestHandler(QJsonValue value);
    void getStationTracksRequestHandler(QJsonValue value);
    void getAccountStatusRequestHandler(QJsonValue value);
    void getStantionsListRequestHandler(QJsonValue value);
    void getStantionsDashboardRequestHandler(QJsonValue value);
    void getStantionFeedbackRequestHandler(QJsonValue value);
    void postStantionFeedbackRequestHandler(QJsonValue value);

private:
    QString m_batchId;
};
Q_DECLARE_METATYPE(Rotor::FeedbackType)
#endif // ROTOR_H
