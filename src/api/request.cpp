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
    , m_debug(false)
{
    m_settings = new Settings();
    m_accessToken = m_settings->value("accessToken").toString();

    if (m_accessToken.isEmpty()) {
        qFatal("Token is empty APP is broken!");
    }

    connect(m_settings, &Settings::settingsUpdated, this, [=] {
        m_accessToken = m_settings->value("accessToken").toString();
    });

    m_request = QNetworkRequest(QUrl(API_URL + m_point));
    m_request.setRawHeader("Authorization", "OAuth " + m_accessToken.toUtf8());
    m_request.setRawHeader("Accept", "*/*");
    m_request.setRawHeader("Connection", "keep-alive");
    m_request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");

    connect(m_manager, &QNetworkAccessManager::finished, this, &Request::replyHandler);
}

void Request::setDebug(bool debug)
{
    m_debug = debug;
}

void Request::get(const QUrlQuery& query)
{
    m_type = "get";
    QUrl requestUrl = m_request.url();
    requestUrl.setQuery(query);
    m_request.setUrl(requestUrl);

    m_manager->get(m_request);
}

void Request::post(const QString& query)
{
    m_type = "post";
    if (query.contains("{")) {
        m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    } else {
        m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
    m_manager->post(m_request, query.toUtf8());
}

void Request::replyHandler(QNetworkReply* reply)
{
    Request* request = qobject_cast<Request*>(sender());

    if (!reply) {
        delete request;
        return;
    }
    reply->deleteLater();

    if (reply->error()) {
        if (m_debug) {
            qDebug() << reply->errorString();
        }
        emit errorReady(reply->errorString());
    } else {
        QString rawAnswer = reply->readAll();
        QJsonObject ansObject = QJsonDocument::fromJson(rawAnswer.toUtf8()).object();

        if (m_debug) {
            qDebug().noquote() << m_request.url().toString() << " GOT ANSWER: " << m_type << rawAnswer;
        }

        if (!ansObject.value("result").isNull()) {
            emit dataReady(ansObject.value("result"));
        } else if (!ansObject.value("error").isNull()) {
            emit errorReady(ansObject.value("error").toString());
        } else {
            qWarning() << "download error!";
        }
    }
    delete request;
}
