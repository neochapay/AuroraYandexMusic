#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "api/feed.h"
#include "api/feedbacksender.h"
#include "api/musicfetcher.h"
#include "api/oauth.h"
#include "api/user.h"

#include "types/cover.h"
#include "types/playlist.h"
#include "types/track.h"

#include "models/currentplaylistmodel.h"

#include <sailfishapp.h>

int main(int argc, char* argv[])
{
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.neochapay"));
    application->setApplicationName(QStringLiteral("yandexmusic"));

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qRegisterMetaType<Artist>("Artist");
    qRegisterMetaType<Cover>("Cover");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<DerivedColors>("DerivedColors");

    qmlRegisterType<Track>();
    qmlRegisterType<Album>();

    qmlRegisterType<CurrentPlayListModel>("ru.neochapay.yandexmusic", 1, 0, "CurrentPlayListModel");
    qmlRegisterType<User>("ru.neochapay.yandexmusic", 1, 0, "User");
    qmlRegisterType<OAuth>("ru.neochapay.yandexmusic", 1, 0, "Auth");
    qmlRegisterType<Feed>("ru.neochapay.yandexmusic", 1, 0, "Feed");
    qmlRegisterType<MusicFetcher>("ru.neochapay.yandexmusic", 1, 0, "MusicFetcher");
    qmlRegisterType<FeedbackSender>("ru.neochapay.yandexmusic", 1, 0, "FeedbackSender");

    view->setSource(SailfishApp::pathTo("qml/YandexMusic.qml"));
    view->show();

    return application->exec();
}
