TEMPLATE = lib
TARGET = qmdnsengine
QT +=  core qml network

SOURCES += \
    abstractserver.cpp \
    bitmap.cpp \
    browser.cpp \
    cache.cpp \
    dns.cpp \
    hostname.cpp \
    mdns.cpp \
    message.cpp \
    prober.cpp \
    provider.cpp \
    query.cpp \
    record.cpp \
    resolver.cpp \
    server.cpp \
    service.cpp \

HEADERS += \
    abstractserver.h \
    bitmap.h \
    bitmap_p.h \
    browser.h \
    browser_p.h \
    cache.h \
    cache_p.h \
    dns.h \
    hostname.h \
    hostname_p.h \
    mdns.h \
    message.h \
    message_p.h \
    prober.h \
    prober_p.h \
    provider.h \
    provider_p.h \
    qmdnsengine_export.h \
    query.h \
    query_p.h \
    record.h \
    record_p.h \
    resolver.h \
    resolver_p.h \
    server.h \
    server_p.h \
    service.h \
    service_p.h

target.path = /usr/share/ru.neochapay.ourmusic/lib

INSTALLS += target
