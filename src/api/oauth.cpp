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

#include "oauth.h"

#include <QStandardPaths>
#include <QUrl>
#include <QUrlQuery>

OAuth::OAuth(QObject* parent)
    : QObject(parent)
{
    m_settings = new Settings();
    m_token = m_settings->value("accessToken").toString();
}

void OAuth::parseUrl(QString url)
{
    QUrl authUrl(url.replace("#", "?"));
    if (!authUrl.isValid() || authUrl.isEmpty()) {
        emit error("Login failed");
        return;
    }

    QUrlQuery q;
    q.setQuery(authUrl.query());
    if (q.isEmpty()) {
        emit error("Login failed");
        return;
    }

    QString accessToken = q.queryItemValue("access_token", QUrl::FullyDecoded);
    QString expiresIn = q.queryItemValue("expires_in", QUrl::FullyDecoded);

    if (accessToken.isEmpty() || expiresIn.isEmpty()) {
        return;
    }

    if (accessToken != m_token) {
        m_token = accessToken;

        m_settings->setValue("accessToken", m_token);
        m_settings->sync();

        emit tokenChanged();
        emit isLoginedChanged();
    }
}

bool OAuth::isLogined() const
{
    return m_token.isEmpty();
}

const QString& OAuth::token() const
{
    return m_token;
}

const QString OAuth::clientID() const
{
    return "23cabbbdc6cd418abb4b39c32c41195d";
}
