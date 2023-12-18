#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "api/feed.h"
#include "api/feedbacksender.h"
#include "api/musicfetcher.h"
#include "api/oauth.h"
#include "api/playlists.h"
#include "api/rotor.h"
#include "api/user.h"
#include "api/landing3.h"

#include "types/cover.h"
#include "types/playlist.h"
#include "types/track.h"

#include "models/currentplaylistmodel.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>

#include <sailfishapp.h>

int main(int argc, char* argv[])
{
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.neochapay"));
    application->setApplicationName(QStringLiteral("ourmusic"));

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qRegisterMetaType<Artist>("Artist");
    qRegisterMetaType<Cover>("Cover");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<DerivedColors>("DerivedColors");

    qmlRegisterType<Track>();
    qmlRegisterType<Album>();

    qmlRegisterType<CurrentPlayListModel>("ru.neochapay.ourmusic", 1, 0, "CurrentPlayListModel");
    qmlRegisterType<User>("ru.neochapay.ourmusic", 1, 0, "User");
    qmlRegisterType<OAuth>("ru.neochapay.ourmusic", 1, 0, "Auth");
    qmlRegisterType<Feed>("ru.neochapay.ourmusic", 1, 0, "Feed");
    qmlRegisterType<MusicFetcher>("ru.neochapay.ourmusic", 1, 0, "MusicFetcher");
    qmlRegisterType<FeedbackSender>("ru.neochapay.ourmusic", 1, 0, "FeedbackSender");
    qmlRegisterType<Playlists>("ru.neochapay.ourmusic", 1, 0, "Playlists");
    qmlRegisterType<Rotor>("ru.neochapay.ourmusic", 1, 0, "Rotor");
    qmlRegisterType<Landing3>("ru.neochapay.ourmusic", 1, 0, "Landing");

    view->setSource(SailfishApp::pathTo("qml/OurMusic.qml"));
    view->show();

    return application->exec();
}
