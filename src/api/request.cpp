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

#include "request.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStandardPaths>

Request::Request(QString point, QObject* parent)
    : QObject(parent)
    , m_point(point)
    , m_manager(new QNetworkAccessManager(this))
{
    m_settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "yamusic.conf", QSettings::NativeFormat);
    m_accessToken = m_settings->value("accessToken").toString();

    m_request = QNetworkRequest(QUrl(API_URL + m_point));
    m_request.setRawHeader("Authorization", "OAuth " + m_accessToken.toUtf8());
    m_request.setRawHeader("Accept", "*/*");
    m_request.setRawHeader("Accept Encoding", "gzip, deflate, sdch, br");
    m_request.setRawHeader("Postman Token", "0602916c-c9be-3364-8938-6b4f5426539e");
    m_request.setRawHeader("Cache Control", "no-cache");
    m_request.setRawHeader("Connection", "keep-alive");
    m_request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");

    connect(m_manager, &QNetworkAccessManager::finished, this, &Request::replyHandler);
}

void Request::get(const QUrlQuery& query)
{
    QUrl requestUrl = m_request.url();
    requestUrl.setQuery(query);
    m_request.setUrl(requestUrl);

    m_manager->get(m_request);
}

void Request::post(const QString& query)
{
    if (query.contains("{")) {
        m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    } else {
        m_request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    }
    m_manager->post(m_request, query.toUtf8());
}

void Request::replyHandler(QNetworkReply* reply)
{
    QString rawAnswer = reply->readAll();
    QJsonObject ansObject = QJsonDocument::fromJson(rawAnswer.toUtf8()).object();

    qDebug().noquote() << m_request.url().toString() << " GOT ANSWER: " << rawAnswer;

    if (!ansObject.take("result").isNull()) {
        emit dataReady(ansObject.take("result").toObject());
        return;
    }

    if (!ansObject.take("error").isNull()) {
        emit errorReady(ansObject.take("error").toObject());
        return;
    }
}
