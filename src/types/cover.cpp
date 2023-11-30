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

#include "cover.h"

Cover::Cover(QObject* parent)
    : QObject(parent)
    , d_ptr(new CoverPrivate)
{
}

Cover::Cover(const Cover& other, QObject* parent)
    : QObject(parent)
    , d_ptr(other.d_ptr)
{
}

Cover::Cover(QJsonObject object, QObject* parent)
    : QObject(parent)
    , d_ptr(new CoverPrivate)
{
    d_ptr->custom = object.value("custom").toBool();
    d_ptr->dir = object.value("dir").toString();
    d_ptr->type = object.value("type").toString();
    d_ptr->uri = object.value("uri").toString();
    d_ptr->version = object.value("version").toInt();
}

Cover::~Cover()
{
    delete d_ptr;
}

Cover& Cover::operator=(const Cover& other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}

bool Cover::operator==(const Cover& other)
{
    return custom() == other.custom()
        && dir() == other.dir()
        && type() == other.type()
        && uri() == other.uri()
        && version() == other.version();
}

bool Cover::custom() const
{
    return d_ptr->custom;
}

const QString& Cover::dir() const
{
    return d_ptr->dir;
}

const QString& Cover::type() const
{
    return d_ptr->type;
}

const QString& Cover::uri() const
{
    return d_ptr->uri;
}

int Cover::version() const
{
    return d_ptr->version;
}
