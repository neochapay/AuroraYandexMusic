TEMPLATE = lib
TARGET = qtwebsockets
QT +=  core network

SOURCES += \
    qdefaultmaskgenerator_p.cpp \
    qmaskgenerator.cpp \
    qsslserver.cpp \
    qwebsocketcorsauthenticator.cpp \
    qwebsocket.cpp \
    qwebsocketdataprocessor.cpp \
    qwebsocketframe.cpp \
    qwebsockethandshakerequest.cpp \
    qwebsockethandshakeresponse.cpp \
    qwebsocket_p.cpp \
    qwebsocketprotocol.cpp \
    qwebsocketserver.cpp \
    qwebsocketserver_p.cpp

HEADERS += \
    qdefaultmaskgenerator_p.h \
    qmaskgenerator.h \
    qsslserver_p.h \
    qwebsocketcorsauthenticator.h \
    qwebsocketcorsauthenticator_p.h \
    qwebsocketdataprocessor_p.h \
    qwebsocketframe_p.h \
    qwebsocket.h \
    qwebsockethandshakerequest_p.h \
    qwebsockethandshakeresponse_p.h \
    qwebsocket_p.h \
    qwebsocketprotocol.h \
    qwebsocketprotocol_p.h \
    qwebsocketserver.h \
    qwebsocketserver_p.h \
    qwebsockets_global.h

target.path = /usr/share/ru.neochapay.ourmusic/lib

INSTALLS += target
