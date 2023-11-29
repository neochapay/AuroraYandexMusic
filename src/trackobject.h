#ifndef TRACKOBJECT_H
#define TRACKOBJECT_H

#include <QObject>

class TrackObject : public QObject {
    Q_OBJECT
public:
    explicit TrackObject(QObject* parent = nullptr);

public:
    int trackId;
    int artistId;
    QString artistName;
    QString artistCover;
    int albumCoverId;
    QString albumName;
    QString albumCover;
    QString trackName;
    QString type;
    int duration;
    QString storageDir;
    bool liked;
    QString fileUrl;
    QString url;
};

#endif // TRACKOBJECT_H
