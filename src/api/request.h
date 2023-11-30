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

#ifndef REQUEST_H
#define REQUEST_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QSettings>
#include <QUrlQuery>

const QString API_URL = "https://api.music.yandex.net";

class Request : public QObject {
    Q_OBJECT

public:
    explicit Request(QString point, QObject* parent = nullptr);
    void setDebug(bool debug);
    void get(const QUrlQuery& query = QUrlQuery());
    void post(const QString& query = "");

signals:
    void dataReady(QJsonValue object);
    void errorReady(QJsonValue object);
    void errorReady(QString message);

private slots:
    void replyHandler(QNetworkReply* reply);

private:
    QString m_point;
    QNetworkRequest m_request;
    QNetworkAccessManager* m_manager;
    QSettings* m_settings;
    QString m_accessToken;
    QString m_type;

    bool m_debug;
};

#endif // REQUEST_H
