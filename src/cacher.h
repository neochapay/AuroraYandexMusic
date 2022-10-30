#ifndef CACHER_H
#define CACHER_H

#include "models/playlistmodel.h"
#include "track.h"
#include <QObject>

class Cacher : public QObject {
    Q_OBJECT

public:
    explicit Cacher(Track* track, QObject* parent = nullptr);
    void saveToCache();
    QString fileToSave();
    QString Url();

signals:
    void fileSaved();

private slots:
    void getDownloadInfoFinished(const QJsonValue& value);
    void getSongUrl();
    void saveData(QByteArray data);

private:
    Track* m_track;
    QString m_fileToSave;
    QString m_Url;
};

#endif // CACHER_H
