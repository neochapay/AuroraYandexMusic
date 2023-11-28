#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "authorization.h"
#include "settings.h"

Authorization::Authorization(QObject* parent)
    : QObject(parent)
{
}

Authorization::~Authorization()
{
}

void Authorization::setupRequest(QNetworkRequest* r)
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();

    r->setRawHeader("Accept", "*/*");
    r->setRawHeader("X-Requested-With", "XMLHttpRequest");
    r->setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    r->setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if (accessToken.size() > 0) {
        r->setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }
}

void Authorization::doAuth(QString username, QString password)
{
    QUrl url(m_oauthURL);
    QUrlQuery q;
    q.addQueryItem("response_type", "token");
    q.addQueryItem("username", username);
    q.addQueryItem("password", password);
    q.addQueryItem("client_id", m_clientID);
    q.addQueryItem("client_secret", m_clientSecret);

    QNetworkRequest r(url);
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    setupRequest(&r);

    QNetworkAccessManager* m = new QNetworkAccessManager;
    QNetworkReply* reply = m->post(r, q.toString().toLatin1());

    connect(reply, &QNetworkReply::finished, this, &Authorization::doAuthFinished);
}

void Authorization::storeToken(QString url)
{
    QUrl authUrl(url.replace("#", "?"));
    if (!authUrl.isValid() || authUrl.isEmpty()) {
        emit error("Login failed");
        return;
    }

    QUrlQuery q;
    q.setQuery(authUrl.query());
    if (q.isEmpty()) {
        emit error("Login failed");
        return;
    }

    QString accessToken = q.queryItemValue("access_token", QUrl::FullyDecoded);
    QString expiresIn = q.queryItemValue("expires_in", QUrl::FullyDecoded);

    if (accessToken.isEmpty() || expiresIn.isEmpty()) {
        emit error("Login failed");
        return;
    }
    m_token = accessToken;
    m_ttl = QDateTime::fromMSecsSinceEpoch(expiresIn.toInt() * 1000);

    Settings settings;
    settings.setValue("accessToken", m_token);
    settings.setValue("ttl", m_ttl);

    emit authorized(m_token);
}

bool Authorization::checkToken()
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();
    QDateTime ttl = settings.value("ttl").toDateTime();

    if (!accessToken.isEmpty()) {
        return true;
    }

    return false;
}

void Authorization::removeAccessToken()
{
    Settings settings;
    settings.remove("accessToken");
}

void Authorization::doAuthFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    const QByteArray info = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(info);
        QJsonObject jo = doc.object();
        if (jo.contains("access_token")) {
            m_token = jo.value("access_token").toString();
            m_ttl = QDateTime::currentDateTime().addSecs(jo.value("expires_in").toInt());

            Settings settings;
            settings.setValue("accessToken", m_token);
            settings.setValue("ttl", m_ttl);
        } else {
            emit error("Strange response");
        }
    } else {
        emit error("Login failed");
    }
}
