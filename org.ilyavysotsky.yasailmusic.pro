TARGET = org.ilyavysotsky.yasailmusic

CONFIG += \
    sailfishapp

PKGCONFIG += \

SOURCES += \
    src/apirequest.cpp \
    src/authorization.cpp \
    src/cacher.cpp \
    src/downloader.cpp \
    src/models/playlistmodel.cpp \
    src/models/searchmodel.cpp \
    src/settings.cpp \
    src/track.cpp \
    src/main.cpp

HEADERS += \
    src/apirequest.h \
    src/authorization.h \
    src/cacher.h \
    src/downloader.h \
    src/models/playlistmodel.h \
    src/models/searchmodel.h \
    src/settings.h \
    src/track.h \
    src/YaSailMusic.h

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
