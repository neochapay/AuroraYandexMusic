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

#ifndef LIKES_H
#define LIKES_H

#include <QJsonValue>
#include <QObject>

class Likes : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID WRITE setUserID NOTIFY userIDChanged)

public:
    explicit Likes(QObject* parent = nullptr);
    Q_INVOKABLE void like(QString type, int id, bool remove = false);
    Q_INVOKABLE void dislike(int id, bool remove = false);

    int userID() const;
    void setUserID(int newUserID);

signals:
    void userIDChanged();
    void finished(int actionId);

private slots:
    void likeRequestHandler(QJsonValue value);
    void dislikeRequestHandler(QJsonValue value);

private:
    int m_userID;
    int m_actionID;
};

#endif // LIKES_H
