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

#include "feedbacksender.h"
#include "request.h"

#include <QDateTime>
#include <QUuid>

FeedbackSender::FeedbackSender(QObject* parent)
    : QObject(parent)
    , m_userID(0)
{
}

void FeedbackSender::sendFeedback(Track* track, int totalSeconds, int playedSeconds, int playlistId)
{
    if (track == nullptr || m_userID == 0) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem("track-id", track->trackId());
    query.addQueryItem("from-cache", "false");
    query.addQueryItem("restored", "true");
    query.addQueryItem("from", "radio-mobile-user-feed-default");
    query.addQueryItem("play-id", QUuid::createUuid().toString().replace("{", "").replace("}", "").toUpper());
    // query.addQueryItem("play-id", "79CFB84C-4A0B-4B31-8954-3006C0BD9274");
    query.addQueryItem("uid", QString::number(m_userID));
    query.addQueryItem("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    query.addQueryItem("track-length-seconds", QString::number(totalSeconds));
    query.addQueryItem("total-played-seconds", QString::number(playedSeconds));
    query.addQueryItem("end-position-seconds", QString::number(playedSeconds));
    if (track->albums().count() > 0) {
        Album* album = dynamic_cast<Album*>(track->albums().first());
        query.addQueryItem("album-id", QString::number(album->albumId()));
    }
    if (playlistId != 0) {
        query.addQueryItem("playlist-id", QString::number(playlistId));
    }
    query.addQueryItem("client-now", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));

    Request* sendFeedbackRequest = new Request("/play-audio");
    sendFeedbackRequest->setDebug(true);
    connect(sendFeedbackRequest, &Request::dataReady, this, &FeedbackSender::sendFeedbackHandler);

    sendFeedbackRequest->post(query.toString());
}

int FeedbackSender::userID() const
{
    return m_userID;
}

void FeedbackSender::setUserID(int newUserID)
{
    if (m_userID == newUserID)
        return;
    m_userID = newUserID;
    emit userIDChanged();
}

void FeedbackSender::sendFeedbackHandler(QJsonValue value)
{
    if (value.toString() == "ok") {
        emit sendFeedbackOk();
    } else {
        emit sendFeedbackFail();
    }
}
