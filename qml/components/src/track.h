#ifndef TRACK_H
#define TRACK_H

#include <QObject>

class Track : public QObject {
    Q_OBJECT
public:
    explicit Track(QObject* parent = nullptr);

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

#endif // TRACK_H
