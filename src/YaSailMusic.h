#ifndef YASAILMUSIC_H
#define YASAILMUSIC_H

#include "trackobject.h"

struct BaseValues {
    BaseValues();
    QList<TrackObject*> currentPlaylist;
};
#define baseValues (*baseValues_)
extern BaseValues* baseValues_;

#endif // YASAILMUSIC_H
