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
    int countsTracks;
    int countsDirectAlbums;
    int countsAlsoAlbums;
    int countsAlsoTracks;
    int likesCount;
    QString ogImage;
    QString name;
    int artistId;
    bool various;
};

class Artist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool composer READ composer NOTIFY artistChanged)
    Q_PROPERTY(QString coverPrefix READ coverPrefix NOTIFY artistChanged)
    Q_PROPERTY(QString coverType READ coverType NOTIFY artistChanged)
    Q_PROPERTY(QString coverUri READ coverUri NOTIFY artistChanged)
    Q_PROPERTY(QStringList disclaimers READ disclaimers NOTIFY artistChanged)
    Q_PROPERTY(QStringList genres READ genres NOTIFY artistChanged)

    Q_PROPERTY(int countsTracks READ countsTracks NOTIFY artistChanged)
    Q_PROPERTY(int countsDirectAlbums READ countsDirectAlbums NOTIFY artistChanged)
    Q_PROPERTY(int countsAlsoAlbums READ countsAlsoAlbums NOTIFY artistChanged)
    Q_PROPERTY(int countsAlsoTracks READ countsAlsoTracks NOTIFY artistChanged)
    Q_PROPERTY(int likesCount READ likesCount NOTIFY artistChanged)
    Q_PROPERTY(QString ogImage READ ogImage NOTIFY artistChanged)

    Q_PROPERTY(QString name READ name NOTIFY artistChanged)
    Q_PROPERTY(int artistId READ artistId NOTIFY artistChanged)
    Q_PROPERTY(bool various READ various NOTIFY artistChanged)

public:
    explicit Artist(QObject* parent = nullptr);
    explicit Artist(const Artist& other, QObject* parent = nullptr);
    explicit Artist(QJsonObject object, QObject* parent = nullptr);
    virtual ~Artist();
    Artist& operator=(const Artist& other);
    bool operator!=(const Artist& other);

    bool composer() const;
    const QString& coverPrefix() const;
    const QString& coverType() const;
    const QString& coverUri() const;
    const QStringList& disclaimers() const;
    const QStringList& genres() const;
    const QString& name() const;
    int artistId() const;
    bool various() const;
    int countsTracks() const;
    int countsDirectAlbums() const;
    int countsAlsoAlbums() const;
    int countsAlsoTracks() const;
    int likesCount() const;
    const QString& ogImage() const;

signals:
    void artistChanged();

private:
    ArtistPrivate* d_ptr;
};

Q_DECLARE_METATYPE(Artist)

#endif // ARTIST_H
