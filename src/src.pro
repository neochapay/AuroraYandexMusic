TARGET = ru.neochapay.ourmusic

CONFIG += \
    sailfishapp

INCLUDEPATH += ../libs/
LIBS += -L../libs/qmdnsengine -lqmdnsengine -L../libs/qtwebsockets -lqtwebsockets

SOURCES += \
    api/albuminfo.cpp \
    api/artistinfo.cpp \
    api/feed.cpp \
    api/feedbacksender.cpp \
    api/landing3.cpp \
    api/musicfetcher.cpp \
    api/oauth.cpp \
    api/playlists.cpp \
    api/request.cpp \
    api/rotor.cpp \
    api/search.cpp \
    api/settings.cpp \
    api/tracks.cpp \
    api/user.cpp \
    alice/stantiondetector.cpp \
    models/currentplaylistmodel.cpp \
    main.cpp \
    types/album.cpp \
    types/artist.cpp \
    types/cover.cpp \
    types/playlist.cpp \
    types/track.cpp

HEADERS += \
    alice/stationdetector.h \
    api/albuminfo.h \
    api/artistinfo.h \
    api/feed.h \
    api/feedbacksender.h \
    api/landing3.h \
    api/musicfetcher.h \
    api/oauth.h \
    api/playlists.h \
    api/request.h \
    api/rotor.h \
    api/search.h \
    api/settings.h \
    api/tracks.h \
    api/user.h \
    alice/stantiondetector.h \
    models/currentplaylistmodel.h \
    types/ArtistInfoData.h \
    types/LandingResultBlock.h \
    types/SearchResultData.h \
    types/album.h \
    types/artist.h \
    types/cover.h \
    types/playlist.h \
    types/track.h


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
    qml/pages/UserLikedTracksPage.qml \
    qml/pages/UserPage.qml \
    qml/pages/UserPlaylistsPage.qml \
    qml/* \
    qml/pages/* \
    qml/covers/* \
    qml/components/*

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

CONFIG += sailfishapp_i18n
greaterThan(QT_MAJOR_VERSION, 5) {
    QT += webview
}

TRANSLATIONS += \
    translations/ru.neochapay.ourmusic-ru.ts \
