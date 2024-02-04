/*
 * Copyright (C) 2023-2024 Chupligin Sergey <neochapay@gmail.com>
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

#ifndef TRACK_H
#define TRACK_H

#include "album.h"
#include "artist.h"

#include <QObject>
struct DerivedColors {
    Q_GADGET
    Q_PROPERTY(QString accent MEMBER accent)
    Q_PROPERTY(QString average MEMBER average)
    Q_PROPERTY(QString miniPlayer MEMBER miniPlayer)
    Q_PROPERTY(QString waveText MEMBER waveText)
public:
    QString accent;
    QString average;
    QString miniPlayer;
    QString waveText;
};

struct Fade {
    Q_GADGET
public:
    float inStart;
    float inStop;
    float outStart;
    float outStop;
};

class TrackPrivate {
public:
    QList<QObject*> albums;
    QList<QObject*> artists;
    bool available;
    QStringList availableForOptions;
    bool availableForPremiumUsers;
    bool availableFullWithoutPermission;
    QString coverUri;
    DerivedColors derivedColors;
    QStringList disclaimers;
    int durationMs;
    Fade fade;
    int fileSize;
    int trackId;
    bool lyricsAvailable;
    QString ogImage;
    int previewDurationMs;
    int realId;
    bool rememberPosition;
    QString storageDir;
    QString title;
    QString trackSharingFlag;
    QString trackSource;
    QString type;
    QString contentWarning;
};

class Track : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> albums READ albums NOTIFY trackChanged)
    Q_PROPERTY(QList<QObject*> artists READ artists NOTIFY trackChanged)
    Q_PROPERTY(bool available READ available NOTIFY trackChanged)
    Q_PROPERTY(QStringList availableForOptions READ availableForOptions NOTIFY trackChanged)
    Q_PROPERTY(bool availableForPremiumUsers READ availableForPremiumUsers NOTIFY trackChanged)
    Q_PROPERTY(bool availableFullWithoutPermission READ availableFullWithoutPermission NOTIFY trackChanged)
    Q_PROPERTY(QString contentWarning READ contentWarning NOTIFY trackChanged)
    Q_PROPERTY(QString coverUri READ coverUri)
    Q_PROPERTY(DerivedColors derivedColors READ derivedColors NOTIFY trackChanged)
    Q_PROPERTY(QStringList disclaimers READ disclaimers NOTIFY trackChanged)
    Q_PROPERTY(int durationMs READ durationMs NOTIFY trackChanged)
    Q_PROPERTY(Fade fade READ fade NOTIFY trackChanged)
    Q_PROPERTY(int fileSize READ fileSize NOTIFY trackChanged)
    Q_PROPERTY(int trackId READ trackId NOTIFY trackChanged)
    Q_PROPERTY(bool lyricsAvailable READ lyricsAvailable NOTIFY trackChanged)
    // TODO lyricsInfo
    // TODO major
    Q_PROPERTY(QString ogImage READ ogImage NOTIFY trackChanged)
    Q_PROPERTY(int previewDurationMs READ previewDurationMs NOTIFY trackChanged)
    // TODO r128
    Q_PROPERTY(int realId READ realId NOTIFY trackChanged)
    Q_PROPERTY(bool rememberPosition READ rememberPosition NOTIFY trackChanged)
    Q_PROPERTY(QString storageDir READ storageDir NOTIFY trackChanged)
    Q_PROPERTY(QString title READ title NOTIFY trackChanged)
    Q_PROPERTY(QString trackSharingFlag READ trackSharingFlag NOTIFY trackChanged)
    Q_PROPERTY(QString trackSource READ trackSource NOTIFY trackChanged)
    Q_PROPERTY(QString type READ type NOTIFY trackChanged)
    Q_PROPERTY(bool downloaded READ downloaded NOTIFY trackChanged)

public:
    explicit Track(QObject* parent = nullptr);
    explicit Track(const Track& other, QObject* parent = nullptr);
    explicit Track(QJsonObject object, QObject* parent = nullptr);
    virtual ~Track();
    Track& operator=(const Track& other);

    const QList<QObject*>& albums() const;
    const QList<QObject*>& artists() const;
    bool available() const;
    const QStringList& availableForOptions() const;
    bool availableForPremiumUsers() const;
    bool availableFullWithoutPermission() const;
    const QString& coverUri() const;
    const DerivedColors& derivedColors() const;
    const QStringList& disclaimers() const;
    int durationMs() const;
    const Fade& fade() const;
    int fileSize() const;
    int trackId() const;
    bool lyricsAvailable() const;
    const QString& ogImage() const;
    int previewDurationMs() const;
    int realId() const;
    bool rememberPosition() const;
    const QString& storageDir() const;
    const QString& title() const;
    const QString& trackSharingFlag() const;
    const QString& trackSource() const;
    const QString& type() const;
    const QString& contentWarning() const;
    bool downloaded();

signals:
    void trackChanged();

private:
    TrackPrivate* d_ptr;
};

Q_DECLARE_METATYPE(Track)
Q_DECLARE_METATYPE(DerivedColors)

#endif // TRACK_H
