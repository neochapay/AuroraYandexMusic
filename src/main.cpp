#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "YaSailMusic.h"
#include "authorization.h"
#include "models/playlistmodel.h"
#include "models/searchmodel.h"
#include "settings.h"
#include "track.h"
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QStandardPaths>
#include <QtGlobal>
#include <sailfishapp.h>
#include <stdio.h>
#include <stdlib.h>

BaseValues* baseValues_;

BaseValues::BaseValues()
{
}

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
    Settings settings;

    baseValues_ = new BaseValues();
    qmlRegisterType<PlaylistModel>("org.ilyavysotsky.yasailmusic", 1, 0, "PlaylistModel");
    qmlRegisterType<SearchModel>("org.ilyavysotsky.yasailmusic", 1, 0, "SearchModel");
    qmlRegisterType<Track>("org.ilyavysotsky.yasailmusic", 1, 0, "Track");

    Authorization* auth = new Authorization();
    view->rootContext()->setContextProperty("application", application.data());
    view->rootContext()->setContextProperty("auth", auth);
    view->setSource(SailfishApp::pathTo("qml/YaSailMusic.qml"));
    view->show();

    return application->exec();
}
