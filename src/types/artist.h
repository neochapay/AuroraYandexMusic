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

class Artist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool composer READ composer WRITE setComposer NOTIFY composerChanged)
    Q_PROPERTY(QString coverPrefix READ coverPrefix WRITE setCoverPrefix NOTIFY coverPrefixChanged)
    Q_PROPERTY(QString coverType READ coverType WRITE setCoverType NOTIFY coverTypeChanged)
    Q_PROPERTY(QString coverUri READ coverUri WRITE setCoverUri NOTIFY coverUriChanged)
    Q_PROPERTY(QStringList disclaimers READ disclaimers WRITE setDisclaimers NOTIFY disclaimersChanged)
    Q_PROPERTY(QStringList genres READ genres WRITE setGenres NOTIFY genresChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(bool various READ various WRITE setVarious NOTIFY variousChanged)

public:
    explicit Artist(QObject* parent = nullptr);
    explicit Artist(QJsonObject object, QObject* parent = nullptr);

    bool composer() const;
    void setComposer(bool newComposer);

    const QString& coverPrefix() const;
    void setCoverPrefix(const QString& newCoverPrefix);

    const QString& coverType() const;
    void setCoverType(const QString& newCoverType);

    const QString& coverUri() const;
    void setCoverUri(const QString& newCoverUri);

    const QStringList& disclaimers() const;
    void setDisclaimers(const QStringList& newDisclaimers);

    const QStringList& genres() const;
    void setGenres(const QStringList& newGenres);

    const QString& name() const;
    void setName(const QString& newName);

    int artistId() const;
    void setArtistId(int newArtistId);

    bool various() const;
    void setVarious(bool newVarious);

signals:
    void composerChanged();
    void coverPrefixChanged();
    void coverTypeChanged();
    void coverUriChanged();
    void disclaimersChanged();
    void genresChanged();
    void nameChanged();
    void artistIdChanged();
    void variousChanged();

private:
    bool m_composer;
    QString m_coverPrefix;
    QString m_coverType;
    QString m_coverUri;
    QStringList m_disclaimers;
    QStringList m_genres;
    QString m_name;
    int m_artistId;
    bool m_various;
};

#endif // ARTIST_H
