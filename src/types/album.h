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

#ifndef ALBUH
#define ALBUH

#include "artist.h"
#include <QDateTime>
#include <QObject>

struct Label {
    Q_GADGET
public:
    int id;
    QString name;
};

class AlbumPrivate {
public:
    QList<QObject*> artists;
    QList<QObject*> tracks;
    bool available;
    bool availableForMobile;
    QStringList availableForOptions;
    bool availableForPremiumUsers;
    bool availablePartially;
    QList<int> bests;
    QString contentWarning;
    QString coverUri;
    QStringList disclaimers;
    QString genere;
    int albumId;
    QList<QObject*> labels;
    int likesConut;
    QString metaType;
    QString orImage;
    bool recent;
    QDateTime releaseDate;
    QString title;
    int trackCount;
    int trackPositionIndex;
    int trackPositionVolume;
    int year;
};

class Album : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> artists READ artists NOTIFY albumChanged)
    Q_PROPERTY(QList<QObject*> tracks READ tracks NOTIFY albumChanged)
    Q_PROPERTY(bool available READ available NOTIFY albumChanged)
    Q_PROPERTY(bool availableForMobile READ availableForMobile NOTIFY albumChanged)
    Q_PROPERTY(QStringList availableForOptions READ availableForOptions NOTIFY albumChanged)
    Q_PROPERTY(bool availableForPremiumUsers READ availableForPremiumUsers NOTIFY albumChanged)
    Q_PROPERTY(bool availablePartially READ availablePartially NOTIFY albumChanged)
    Q_PROPERTY(QList<int> bests READ bests NOTIFY albumChanged)
    Q_PROPERTY(QString contentWarning READ contentWarning NOTIFY albumChanged)
    Q_PROPERTY(QString coverUri READ coverUri NOTIFY albumChanged)
    Q_PROPERTY(QStringList disclaimers READ disclaimers NOTIFY albumChanged)
    Q_PROPERTY(QString genere READ genere NOTIFY albumChanged)
    Q_PROPERTY(int albumId READ albumId NOTIFY albumChanged)
    Q_PROPERTY(QList<QObject*> labels READ labels NOTIFY albumChanged)
    Q_PROPERTY(int likesConut READ likesConut NOTIFY albumChanged)
    Q_PROPERTY(QString metaType READ metaType NOTIFY albumChanged)
    Q_PROPERTY(QString orImage READ orImage NOTIFY albumChanged)
    Q_PROPERTY(bool recent READ recent NOTIFY albumChanged)
    Q_PROPERTY(QDateTime releaseDate READ releaseDate NOTIFY albumChanged)
    Q_PROPERTY(QString title READ title NOTIFY albumChanged)
    Q_PROPERTY(int trackCount READ trackCount NOTIFY albumChanged)
    Q_PROPERTY(int trackPositionIndex READ trackPositionIndex NOTIFY albumChanged)
    Q_PROPERTY(int trackPositionVolume READ trackPositionVolume NOTIFY albumChanged)
    Q_PROPERTY(int year READ year NOTIFY albumChanged)

public:
    explicit Album(QObject* parent = nullptr);
    explicit Album(const Album& other, QObject* parent = nullptr);
    explicit Album(QJsonObject object, QObject* parent = nullptr);
    virtual ~Album();
    Album& operator=(const Album& other);

    const QList<QObject*>& artists() const;
    bool available() const;
    bool availableForMobile() const;
    const QStringList& availableForOptions() const;
    bool availableForPremiumUsers() const;
    bool availablePartially() const;
    const QList<int>& bests() const;
    const QString& contentWarning() const;
    const QString& coverUri() const;
    const QStringList& disclaimers() const;
    const QString& genere() const;
    int albumId() const;
    const QList<QObject*> labels() const;
    int likesConut() const;
    const QString& metaType() const;
    const QString& orImage() const;
    bool recent() const;
    const QDateTime& releaseDate() const;
    const QString& title() const;
    int trackCount() const;
    int trackPositionIndex() const;
    int trackPositionVolume() const;
    int year() const;

    const QList<QObject *> &tracks() const;

signals:
    void albumChanged();

private:
    AlbumPrivate* d_ptr;
};

Q_DECLARE_METATYPE(Album)

#endif // ALBUH
