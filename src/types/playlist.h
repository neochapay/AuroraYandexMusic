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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "cover.h"
#include <QDateTime>
#include <QObject>

class Playlist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QString backgroundImageUrl READ backgroundImageUrl WRITE setBackgroundImageUrl NOTIFY backgroundImageUrlChanged)
    Q_PROPERTY(QString backgroundVideoUrl READ backgroundVideoUrl WRITE setBackgroundVideoUrl NOTIFY backgroundVideoUrlChanged)
    Q_PROPERTY(Cover cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(Cover coverWithoutText READ coverWithoutText WRITE setCoverWithoutText NOTIFY coverWithoutTextChanged)
    Q_PROPERTY(QDateTime created READ created WRITE setCreated NOTIFY createdChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString descriptionFormatted READ descriptionFormatted WRITE setDescriptionFormatted NOTIFY descriptionFormattedChanged)
    Q_PROPERTY(int durationMs READ durationMs WRITE setDurationMs NOTIFY durationMsChanged)
    Q_PROPERTY(bool everPlayed READ everPlayed WRITE setEverPlayed NOTIFY everPlayedChanged)
    Q_PROPERTY(QString generatedPlaylistType READ generatedPlaylistType WRITE setGeneratedPlaylistType NOTIFY generatedPlaylistTypeChanged)
    Q_PROPERTY(QString idForFrom READ idForFrom WRITE setIdForFrom NOTIFY idForFromChanged)
    Q_PROPERTY(bool isBanner READ isBanner WRITE setIsBanner NOTIFY isBannerChanged)
    Q_PROPERTY(bool isPremiere READ isPremiere WRITE setIsPremiere NOTIFY isPremiereChanged)
    Q_PROPERTY(int kind READ kind WRITE setKind NOTIFY kindChanged)
    // TODO: add madeFor
    Q_PROPERTY(QDateTime modified READ modified WRITE setModified NOTIFY modifiedChanged)
    Q_PROPERTY(QString ogImage READ ogImage WRITE setOgImage NOTIFY ogImageChanged)
    // TODO: add owner
    Q_PROPERTY(QString playlistUuid READ playlistUuid WRITE setPlaylistUuid NOTIFY playlistUuidChanged)
    Q_PROPERTY(int revision READ revision WRITE setRevision NOTIFY revisionChanged)
    Q_PROPERTY(int snapshot READ snapshot WRITE setSnapshot NOTIFY snapshotChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int trackCount READ trackCount WRITE setTrackCount NOTIFY trackCountChanged)
    // TODO: add tracks

public:
    explicit Playlist(QObject* parent = nullptr);
    explicit Playlist(QJsonObject object, QObject* parent = nullptr);

    bool available() const;
    void setAvailable(bool newAvailable);

    const QString& backgroundImageUrl() const;
    void setBackgroundImageUrl(const QString& newBackgroundImageUrl);

    const QString& backgroundVideoUrl() const;
    void setBackgroundVideoUrl(const QString& newBackgroundVideoUrl);

    const Cover& cover() const;
    void setCover(const Cover& newCover);

    const Cover& coverWithoutText() const;
    void setCoverWithoutText(const Cover& newCoverWithoutText);

    const QDateTime& created() const;
    void setCreated(const QDateTime& newCreated);

    const QString& description() const;
    void setDescription(const QString& newDescription);

    const QString& descriptionFormatted() const;
    void setDescriptionFormatted(const QString& newDescriptionFormatted);

    int durationMs() const;
    void setDurationMs(int newDurationMs);

    bool everPlayed() const;
    void setEverPlayed(bool newEverPlayed);

    const QString& generatedPlaylistType() const;
    void setGeneratedPlaylistType(const QString& newGeneratedPlaylistType);

    const QString& idForFrom() const;
    void setIdForFrom(const QString& newIdForFrom);

    bool isBanner() const;
    void setIsBanner(bool newIsBanner);

    bool isPremiere() const;
    void setIsPremiere(bool newIsPremiere);

    int kind() const;
    void setKind(int newKind);

    const QDateTime& modified() const;
    void setModified(const QDateTime& newModified);

    const QString& ogImage() const;
    void setOgImage(const QString& newOgImage);

    const QString& playlistUuid() const;
    void setPlaylistUuid(const QString& newPlaylistUuid);

    int revision() const;
    void setRevision(int newRevision);

    int snapshot() const;
    void setSnapshot(int newSnapshot);

    const QStringList& tags() const;
    void setTags(const QStringList& newTags);

    const QString& title() const;
    void setTitle(const QString& newTitle);

    int trackCount() const;
    void setTrackCount(int newTrackCount);

signals:
    void availableChanged();
    void backgroundImageUrlChanged();
    void backgroundVideoUrlChanged();
    void coverChanged();
    void coverWithoutTextChanged();
    void createdChanged();
    void descriptionChanged();
    void descriptionFormattedChanged();
    void durationMsChanged();
    void everPlayedChanged();
    void generatedPlaylistTypeChanged();
    void idForFromChanged();
    void isBannerChanged();
    void isPremiereChanged();
    void kindChanged();
    void modifiedChanged();
    void ogImageChanged();
    void playlistUuidChanged();
    void revisionChanged();
    void snapshotChanged();
    void tagsChanged();
    void titleChanged();
    void trackCountChanged();

private:
    bool m_available;
    QString m_backgroundImageUrl;
    QString m_backgroundVideoUrl;
    Cover m_cover;
    Cover m_coverWithoutText;
    QDateTime m_created;
    QString m_description;
    QString m_descriptionFormatted;
    int m_durationMs;
    bool m_everPlayed;
    QString m_generatedPlaylistType;
    QString m_idForFrom;
    bool m_isBanner;
    bool m_isPremiere;
    int m_kind;
    QDateTime m_modified;
    QString m_ogImage;
    QString m_playlistUuid;
    int m_revision;
    int m_snapshot;
    QStringList m_tags;
    QString m_title;
    int m_trackCount;
};

#endif // PLAYLIST_H
