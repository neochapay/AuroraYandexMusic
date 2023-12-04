TARGET = ru.neochapay.yandexmusic

CONFIG += \
    sailfishapp

PKGCONFIG += \

SOURCES += \
    src/api/feed.cpp \
    src/api/musicfetcher.cpp \
    src/api/oauth.cpp \
    src/api/request.cpp \
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
    src/api/feed.h \
    src/api/musicfetcher.h \
    src/api/oauth.h \
    src/api/request.h \
    src/api/settings.h \
    src/api/user.h \
    src/models/currentplaylistmodel.h \
    src/types/album.h \
    src/types/artist.h \
    src/types/cover.h \
    src/types/playlist.h \
    src/types/track.h


DISTFILES += \
    qml/components/BigPlayer.qml \
    qml/components/LittlePlayer.qml \
    qml/components/MainPlayer.qml \
    qml/components/PlaylistCoverFeed.qml \
    qml/pages/FeedPage.qml \
    rpm/*.spec \
    qml/* \
    qml/pages/* \
    qml/covers/* \
    qml/components/* \
    LICENSE \
    README.md \

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/ru.neochapay.yandexmusic-ru.ts \
