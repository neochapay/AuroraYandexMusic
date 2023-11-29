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

#ifndef ARTIST_H
#define ARTIST_H

#include <QJsonObject>
#include <QObject>

class ArtistPrivate {
public:
    bool composer;
    QString coverPrefix;
    QString coverType;
    QString coverUri;
    QStringList disclaimers;
    QStringList genres;
    QString name;
    int artistId;
    bool various;
};

class Artist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool composer READ composer)
    Q_PROPERTY(QString coverPrefix READ coverPrefix)
    Q_PROPERTY(QString coverType READ coverType)
    Q_PROPERTY(QString coverUri READ coverUri)
    Q_PROPERTY(QStringList disclaimers READ disclaimers)
    Q_PROPERTY(QStringList genres READ genres)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(int artistId READ artistId)
    Q_PROPERTY(bool various READ various)

public:
    explicit Artist(const Artist& other, QObject* parent = nullptr);
    explicit Artist(QJsonObject object, QObject* parent = nullptr);
    virtual ~Artist();
    Artist& operator=(const Artist& other);

    bool composer() const;
    const QString& coverPrefix() const;
    const QString& coverType() const;
    const QString& coverUri() const;
    const QStringList& disclaimers() const;
    const QStringList& genres() const;
    const QString& name() const;
    int artistId() const;
    bool various() const;

private:
    ArtistPrivate* d_ptr;
};

#endif // ARTIST_H
