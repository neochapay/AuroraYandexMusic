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

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QString>

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

    // Station info & tracks
    Q_INVOKABLE void getStationInfo(const QString &stationId = QStringLiteral("user:onyourwave"));
    Q_INVOKABLE void getStationTracks(const QString &stationId = QStringLiteral("user:onyourwave"), int lastTrackid = -1);
    Q_INVOKABLE void getStationTracks(const QString &stationId, const QString &queueTrackId, bool settings2 = true);

    // Account/stations
    Q_INVOKABLE void getAccountStatus();
    Q_INVOKABLE void getStationsList(const QString &language = QString());
    Q_INVOKABLE void getStationsDashboard();
    // Compatibility aliases (typos)
    Q_INVOKABLE void getStantionsList();
    Q_INVOKABLE void getStantionsDashboard();

    // Feedback
    Q_INVOKABLE void postStationFeedback(FeedbackType type,
                                         const QString &stationId = QStringLiteral("user:onyourwave"),
                                         const QString &trackId = QString(),
                                         int totalPlayedSeconds = 0,
                                         const QString &from = QString(),
                                         const QString &batchId = QString());
    Q_INVOKABLE void postStationFeedback(FeedbackType type, Track* track, const QString &stationId = QStringLiteral("user:onyourwave"), int totalPlayedSeconds = 0);
    Q_INVOKABLE void postStantionFeedback(FeedbackType type, Track* track, const QString &stationId = QStringLiteral("user:onyourwave"), int totalPlayedSeconds = 0); // compatibility

    QString batchId() const { return m_batchId; }

signals:
    // New signals
    void stationInfoReady(const QString &stationId, const QJsonValue &info);
    void stationTracksReady(const QString &stationId, const QList<QObject*> &tracks, const QString &batchId, const QJsonObject &rawResult);
    void accountStatusReady(const QJsonValue &status);
    void stationsListReady(const QJsonArray &stations);
    void stationsDashboardReady(const QJsonValue &dashboard);
    void stationFeedbackSent(const QString &type, const QJsonValue &result);
    void error(const QString &message);

    // Compatibility (existing QML expects this name/signature)
    void stantionTracksReady(QList<QObject*> tracks);

private slots:
    void getStationInfoRequestHandler(QJsonValue value);
    void getStationTracksRequestHandler(QJsonValue value);
    void getAccountStatusRequestHandler(QJsonValue value);
    void getStantionsListRequestHandler(QJsonValue value);
    void getStantionsDashboardRequestHandler(QJsonValue value);
    void postStantionFeedbackRequestHandler(QJsonValue value);

private:
    static QString feedbackTypeToString(FeedbackType type);
    QString trackFullId(Track* track) const;

    QString m_batchId;
    QString m_lastStationId;
    QString m_lastFeedbackType;
};

Q_DECLARE_METATYPE(Rotor::FeedbackType)
#endif // ROTOR_H

