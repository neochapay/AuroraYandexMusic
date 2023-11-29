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

#ifndef COVER_H
#define COVER_H

#include <QJsonObject>
#include <QObject>

class CoverPrivate {
public:
    CoverPrivate() {};
    bool custom;
    QString dir;
    QString type;
    QString uri;
    int version;
};

class Cover : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool custom READ custom)
    Q_PROPERTY(QString dir READ dir)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString uri READ uri)
    Q_PROPERTY(int version READ version)

public:
    explicit Cover(const Cover& other, QObject* parent = nullptr);
    explicit Cover(QJsonObject object, QObject* parent = nullptr);
    virtual ~Cover();
    Cover& operator=(const Cover& other);
    bool operator==(const Cover& other);

    bool custom() const;
    const QString& dir() const;
    const QString& type() const;
    const QString& uri() const;
    int version() const;

private:
    CoverPrivate* d_ptr;
};

#endif // COVER_H
