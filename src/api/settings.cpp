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

#include "settings.h"

#include <QStandardPaths>
#include <QDebug>
#include <QFile>

Settings::Settings(QSettings* parent)
    : QSettings (QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/yamusic.conf", QSettings::NativeFormat, parent)
    , m_settingsFileWatcher(new QFileSystemWatcher(this))
{
    m_settingsFileWatcher->addPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/yamusic.conf");
    connect(m_settingsFileWatcher, &QFileSystemWatcher::fileChanged, this, &Settings::settingsUpdated);
}
