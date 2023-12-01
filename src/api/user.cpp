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

void User::getAccountStatusHandler(QJsonValue value)
{
    QJsonObject accountObject = value.toObject().value("account").toObject();
    QString dName = accountObject.value("displayName").toString();
    if (dName != m_displayName) {
        m_displayName = dName;
        emit displayNameChanged();
    }

    int userId = accountObject.value("uid").toInt();

    if (userId == 0) {
        qWarning() << "uers ID is null";
        emit wrongUser();
    }

    if (userId != m_userID) {
        m_userID = userId;
        emit userIDChanged();
    }
}

void User::getFeedHandler(QJsonValue value)
{
    qDebug() << value;
}

int User::userID() const
{
    return m_userID;
}

const QString& User::displayName() const
{
    return m_displayName;
}