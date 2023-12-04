#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "api/feed.h"
#include "api/musicfetcher.h"
#include "api/oauth.h"
#include "api/user.h"

#include "types/cover.h"
#include "types/playlist.h"
#include "types/track.h"

#include "models/currentplaylistmodel.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QStandardPaths>
#include <QtGlobal>
#include <sailfishapp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // SailfishApp::main() will display "qml/YandexMusic.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.neochapay"));
    application->setApplicationName(QStringLiteral("yandexmusic"));

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qRegisterMetaType<Album>("Album");
    qRegisterMetaType<Artist>("Artist");
    qRegisterMetaType<Cover>("Cover");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<Track>("Track");
    qRegisterMetaType<DerivedColors>("DerivedColors");

    qmlRegisterType<Track>();

    qmlRegisterType<CurrentPlayListModel>("ru.neochapay.yandexmusic", 1, 0, "CurrentPlayListModel");
    qmlRegisterType<User>("ru.neochapay.yandexmusic", 1, 0, "User");
    qmlRegisterType<OAuth>("ru.neochapay.yandexmusic", 1, 0, "Auth");
    qmlRegisterType<Feed>("ru.neochapay.yandexmusic", 1, 0, "Feed");
    qmlRegisterType<MusicFetcher>("ru.neochapay.yandexmusic", 1, 0, "MusicFetcher");

    view->setSource(SailfishApp::pathTo("qml/YandexMusic.qml"));
    view->show();

    return application->exec();
}
