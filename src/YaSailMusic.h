#ifndef YASAILMUSIC_H
#define YASAILMUSIC_H

#include "track.h"

struct BaseValues {
    BaseValues();
    QList<Track*> currentPlaylist;
};
#define baseValues (*baseValues_)
extern BaseValues* baseValues_;

#endif // YASAILMUSIC_H
