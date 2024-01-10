/*
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "api/albuminfo.h"
#include "api/artistinfo.h"
#include "api/feed.h"
#include "api/feedbacksender.h"
#include "api/landing3.h"
#include "api/musicfetcher.h"
#include "api/oauth.h"
#include "api/playlists.h"
#include "api/rotor.h"
#include "api/search.h"
#include "api/user.h"

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
    qRegisterMetaType<Search::SearchType>("SearchType");
    qRegisterMetaType<Rotor::FeedbackType>("FeedbackType");

    qmlRegisterType<CurrentPlayListModel>("ru.neochapay.ourmusic", 1, 0, "CurrentPlayListModel");
    qmlRegisterType<User>("ru.neochapay.ourmusic", 1, 0, "User");
    qmlRegisterType<OAuth>("ru.neochapay.ourmusic", 1, 0, "Auth");
    qmlRegisterType<Feed>("ru.neochapay.ourmusic", 1, 0, "Feed");
    qmlRegisterType<MusicFetcher>("ru.neochapay.ourmusic", 1, 0, "MusicFetcher");
    qmlRegisterType<FeedbackSender>("ru.neochapay.ourmusic", 1, 0, "FeedbackSender");
    qmlRegisterType<Playlists>("ru.neochapay.ourmusic", 1, 0, "Playlists");
    qmlRegisterType<Rotor>("ru.neochapay.ourmusic", 1, 0, "Rotor");
    qmlRegisterType<Landing3>("ru.neochapay.ourmusic", 1, 0, "Landing");
    qmlRegisterType<Search>("ru.neochapay.ourmusic", 1, 0, "Search");
    qmlRegisterType<ArtistInfo>("ru.neochapay.ourmusic", 1, 0, "ArtistInfo");
    qmlRegisterType<AlbumInfo>("ru.neochapay.ourmusic", 1, 0, "AlbumInfo");

    view->setSource(SailfishApp::pathTo("qml/OurMusic.qml"));
    view->show();

    return application->exec();
}
