#include "user.h"
#include "apirequest.h"

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
    ApiRequest* getAccoutStatusApiRequest = new ApiRequest();
    connect(getAccoutStatusApiRequest, &ApiRequest::gotResponse, this, &User::getAccountStatusHandler);

    getAccoutStatusApiRequest->makeApiGetRequest("/account/status");
}

void User::getFeed()
{
    ApiRequest* getFeedApiRequest = new ApiRequest();
    connect(getFeedApiRequest, &ApiRequest::gotResponse, this, &User::getFeedHandler);

    getFeedApiRequest->makeApiGetRequest("/feed");
}

void User::getAccountStatusHandler(QJsonValue& value)
{
    QJsonObject accountObject = value.toObject().take("account").toObject();
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

void User::getFeedHandler(QJsonValue& value)
{
    qDebug() << value.toString();
}

int User::userID() const
{
    return m_userID;
}

const QString& User::displayName() const
{
    return m_displayName;
}
