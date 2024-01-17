TARGET = ru.neochapay.ourmusic

CONFIG += \
    sailfishapp

PKGCONFIG += \

SOURCES += \
    src/api/albuminfo.cpp \
    src/api/artistinfo.cpp \
    src/api/feed.cpp \
    src/api/feedbacksender.cpp \
    src/api/landing3.cpp \
    src/api/musicfetcher.cpp \
    src/api/oauth.cpp \
    src/api/playlists.cpp \
    src/api/request.cpp \
    src/api/rotor.cpp \
    src/api/search.cpp \
    src/api/settings.cpp \
    src/api/user.cpp \
    src/models/currentplaylistmodel.cpp \
    src/main.cpp \
    src/types/album.cpp \
    src/types/artist.cpp \
    src/types/cover.cpp \
    src/types/playlist.cpp \
    src/types/track.cpp

HEADERS += \
    src/api/albuminfo.h \
    src/api/artistinfo.h \
    src/api/feed.h \
    src/api/feedbacksender.h \
    src/api/landing3.h \
    src/api/musicfetcher.h \
    src/api/oauth.h \
    src/api/playlists.h \
    src/api/request.h \
    src/api/rotor.h \
    src/api/search.h \
    src/api/settings.h \
    src/api/user.h \
    src/models/currentplaylistmodel.h \
    src/types/ArtistInfoData.h \
    src/types/LandingResultBlock.h \
    src/types/SearchResultData.h \
    src/types/album.h \
    src/types/artist.h \
    src/types/cover.h \
    src/types/playlist.h \
    src/types/track.h


DISTFILES += \
    qml/components/AlbumCoverFeed.qml \
    qml/components/AlbumListItemDelegate.qml \
    qml/components/ArtistListItemDelegate.qml \
    qml/components/BigPlayer.qml \
    qml/components/FeedPage/NewReleasesBlock.qml \
    qml/components/FeedPage/PersonalPlaylistsBlock.qml \
    qml/components/LittlePlayer.qml \
    qml/components/MainPlayer.qml \
    qml/components/MprisController.qml \
    qml/components/MyWavePlayer.qml \
    qml/components/PlaylistCoverFeed.qml \
    qml/components/SvgIcon.qml \
    qml/components/TrackListItemDelegate.qml \
    qml/pages/AlbumPage.qml \
    qml/pages/ArtistPage.qml \
    qml/pages/FeedPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/UserPage.qml \
    qml/pages/UserPlaylistsPage.qml \
    rpm/*.spec \
    qml/* \
    qml/pages/* \
    qml/covers/* \
    qml/components/* \
    LICENSE \
    README.md \
    ru.neochapay.ourmusic.desktop_nojail

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/ru.neochapay.ourmusic-ru.ts \
