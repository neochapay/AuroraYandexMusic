TARGET = org.ilyavysotsky.yasailmusic

CONFIG += \
    sailfishapp

PKGCONFIG += \

SOURCES += \
    src/api/oauth.cpp \
    src/api/request.cpp \
    src/cacher.cpp \
    src/downloader.cpp \
    src/models/playlistmodel.cpp \
    src/models/searchmodel.cpp \
    src/main.cpp \
    src/trackobject.cpp \
    src/types/album.cpp \
    src/types/artist.cpp \
    src/types/cover.cpp \
    src/types/playlist.cpp \
    src/types/track.cpp \
    src/user.cpp

HEADERS += \
    src/api/oauth.h \
    src/api/request.h \
    src/cacher.h \
    src/downloader.h \
    src/models/playlistmodel.h \
    src/models/searchmodel.h \
    src/trackobject.h \
    src/types/album.h \
    src/types/artist.h \
    src/types/cover.h \
    src/types/playlist.h \
    src/types/track.h \
    src/user.h

DISTFILES += \
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
    translations/org.ilyavysotsky.yasailmusic-ru.ts \
