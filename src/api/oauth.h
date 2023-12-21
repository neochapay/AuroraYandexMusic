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
#ifndef OAUTH_H
#define OAUTH_H

#include "settings.h"

#include <QObject>

class OAuth : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isLogined READ isLogined NOTIFY isLoginedChanged)
    Q_PROPERTY(QString token READ token NOTIFY tokenChanged)
    Q_PROPERTY(QString clientID READ clientID)

public:
    explicit OAuth(QObject* parent = nullptr);
    Q_INVOKABLE void parseUrl(QString url);

    bool isLogined() const;
    const QString& token() const;

    const QString clientID() const;

signals:
    void isLoginedChanged();
    void tokenChanged();

    void error(QString errorMessage);

private:
    bool m_isLogined;
    QString m_token;
    Settings* m_settings;
    QString m_clientID;
};

#endif // OAUTH_H
