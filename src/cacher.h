#ifndef CACHER_H
#define CACHER_H

#include "models/playlistmodel.h"
#include "track.h"
#include <QObject>

class Cacher : public QObject {
    Q_OBJECT
    Q_PROPERTY(int trackId READ trackId WRITE setTrackId NOTIFY trackIdChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)

public:
    explicit Cacher(QObject* parent = nullptr);
    void setTrack(Track* track);
    Q_INVOKABLE void saveToCache();
    QString Url();
    int artistId() {return m_artistId;}
    int trackId() {return m_trackId;}
    void setTrackId(int id);
    void setArtistId(int id);

signals:
    void fileSaved(QString path);
    void trackIdChanged();
    void artistIdChanged();

private slots:
    void getDownloadInfoFinished(const QJsonValue& value);
    void getSongUrl();
    void saveData(QByteArray data);

private:
    int m_artistId;
    int m_trackId;

    QString m_fileToSave;
    QString m_Url;
};

#endif // CACHER_H
