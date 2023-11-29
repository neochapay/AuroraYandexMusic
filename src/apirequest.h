#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE
class QHttpMultiPart;
class QNetworkAccessManager;
class QNetworkReply;
class QJsonValue;
QT_END_NAMESPACE

class ApiRequest : public QObject {
    Q_OBJECT

public:
    explicit ApiRequest(QObject* parent = 0);
    ~ApiRequest();

    void makeApiGetRequest(const QString& method, const QUrlQuery& query = QUrlQuery());
    void makeApiPostRequest(const QString& method, const QString& query);

signals:
    void gotResponse(QJsonValue& value);

public slots:
    void dataReady(QNetworkReply* reply);

private:
    const QString API_URL = "https://api.music.yandex.net";

    QNetworkAccessManager* _manager;
};

#endif // APIREQUEST_H
