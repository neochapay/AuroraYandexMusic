#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "api/feed.h"
#include "api/oauth.h"

#include "types/cover.h"
#include "types/playlist.h"
#include "types/track.h"

#include "cacher.h"
#include "models/currentplaylistmodel.h"
#include "models/searchmodel.h"
#include "trackobject.h"
#include "user.h"

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
    application->setOrganizationName(QStringLiteral("org.ilyavysotsky"));
    application->setApplicationName(QStringLiteral("yasailmusic"));

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qRegisterMetaType<Album>("Album");
    qRegisterMetaType<Artist>("Artist");
    qRegisterMetaType<Cover>("Cover");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<Track>("Track");

    qmlRegisterType<SearchModel>("org.ilyavysotsky.yasailmusic", 1, 0, "SearchModel");
    qmlRegisterType<CurrentPlayListModel>("org.ilyavysotsky.yasailmusic", 1, 0, "CurrentPlayListModel");
    qmlRegisterType<Cacher>("org.ilyavysotsky.yasailmusic", 1, 0, "Cacher");
    qmlRegisterType<User>("org.ilyavysotsky.yasailmusic", 1, 0, "User");
    qmlRegisterType<OAuth>("org.ilyavysotsky.yasailmusic", 1, 0, "Auth");
    qmlRegisterType<Feed>("org.ilyavysotsky.yasailmusic", 1, 0, "Feed");

    view->setSource(SailfishApp::pathTo("qml/YaSailMusic.qml"));
    view->show();

    return application->exec();
}
