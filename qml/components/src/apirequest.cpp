#include <QElapsedTimer>
#include <QGuiApplication>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include "apirequest.h"
#include "settings.h"

ApiRequest::ApiRequest(QObject* parent)
    : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
}

ApiRequest::~ApiRequest()
{
    delete _manager;
}

void delay(int ms)
{
    QElapsedTimer et;
    et.start();
    while (true) {
        QGuiApplication::processEvents();
        if (et.elapsed() > ms)
            break;
    }
}

void ApiRequest::makeApiGetRequest(const QString& method, const QUrlQuery& q)
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();

    QUrlQuery query = q;
    QUrl url(API_URL + method);
    url.setQuery(query.query());
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "OAuth " + accessToken.toUtf8());
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
    request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");

    connect(_manager, &QNetworkAccessManager::finished, this, &ApiRequest::dataReady, Qt::UniqueConnection);
    _manager->get(request);
}

void ApiRequest::makeApiPostRequest(const QString& method, const QString& q)
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();

    QString query = q;
    QUrl url(API_URL + method);
    qDebug() << url.toString();
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "OAuth " + accessToken.toUtf8());
    if (query.contains("{")) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    } else {
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    }
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
    request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");
    connect(_manager, &QNetworkAccessManager::finished, this, &ApiRequest::dataReady, Qt::UniqueConnection);
    _manager->post(request, query.toLatin1());
}

void ApiRequest::dataReady(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(data);
    QJsonObject jObj = jDoc.object();
    QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
    if (strFromObj.contains("result")) {
        QJsonValue jVal = jObj.value("result");
        qDebug() << "Reply: " << strFromObj;
        emit gotResponse(jVal);
    } else {
        qDebug() << "Error in API request!";
        qDebug() << "+++++++++++++++++++";
        qDebug() << data;
    }
}
