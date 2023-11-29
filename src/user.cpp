#include "user.h"
#include "api/request.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

User::User(QObject* parent)
    : QObject(parent)
    , m_userID(-1)
    , m_displayName("Loading..")
{
}

void User::getAccountStatus()
{
    Request* getAccoutStatusApiRequest = new Request("/account/status");
    connect(getAccoutStatusApiRequest, &Request::dataReady, this, &User::getAccountStatusHandler);

    getAccoutStatusApiRequest->get();
}

void User::getFeed()
{
    Request* getFeedApiRequest = new Request("/feed");
    connect(getFeedApiRequest, &Request::dataReady, this, &User::getFeedHandler);

    getFeedApiRequest->get();
}

void User::getAccountStatusHandler(QJsonObject object)
{
    QJsonObject accountObject = object.take("account").toObject();
    QString dName = accountObject.take("displayName").toString();
    if (dName != m_displayName) {
        m_displayName = dName;
        emit displayNameChanged();
    }

    int userId = accountObject.take("uid").toInt();
    if (userId != m_userID) {
        m_userID = userId;
        emit userIDChanged();
    }
}

void User::getFeedHandler(QJsonObject object)
{
    qDebug() << object;
}

int User::userID() const
{
    return m_userID;
}

const QString& User::displayName() const
{
    return m_displayName;
}
