#include "stationcontroller.h"

#include "../api/settings.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QSslError>
#include <QUrlQuery>
#include <QUuid>

StationController::StationController(QObject *parent)
    : QObject(parent)
{
    connect(&m_socket, &QWebSocket::connected, this, &StationController::onConnected);
    connect(&m_socket, &QWebSocket::disconnected, this, &StationController::onDisconnected);
    connect(&m_socket, &QWebSocket::textMessageReceived, this, &StationController::onTextMessageReceived);
    connect(&m_socket, &QWebSocket::errorOccurred, this, &StationController::onError);
    connect(&m_socket, &QWebSocket::sslErrors, this, &StationController::onSslErrors);
}

StationController::~StationController()
{
    if (m_connected || m_socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject payload;
        payload.insert(QStringLiteral("command"), QStringLiteral("stop"));

        QJsonObject message;
        QString requestId = QUuid::createUuid().toString();
        requestId.remove(QLatin1Char('{'));
        requestId.remove(QLatin1Char('}'));
        message.insert(QStringLiteral("conversationToken"), m_deviceToken);
        message.insert(QStringLiteral("id"), requestId);
        message.insert(QStringLiteral("payload"), payload);
        message.insert(QStringLiteral("sentTime"), static_cast<double>(QDateTime::currentMSecsSinceEpoch()));

        const QString msg = QString::fromUtf8(QJsonDocument(message).toJson(QJsonDocument::Compact));
        m_socket.sendTextMessage(msg);
        m_socket.flush();
    }
    m_socket.abort();
}

bool StationController::connected() const
{
    return m_connected;
}

QString StationController::status() const
{
    return m_status;
}

bool StationController::remotePlaying() const
{
    return m_remotePlaying;
}

qreal StationController::remoteProgress() const
{
    return m_remoteProgress;
}

qreal StationController::remoteDuration() const
{
    return m_remoteDuration;
}

qreal StationController::remoteVolume() const
{
    return m_remoteVolume;
}

QString StationController::remoteTrackId() const
{
    return m_remoteTrackId;
}

QString StationController::remoteTrackTitle() const
{
    return m_remoteTrackTitle;
}

QString StationController::remoteTrackArtist() const
{
    return m_remoteTrackArtist;
}

QString StationController::remoteTrackCover() const
{
    return m_remoteTrackCover;
}

void StationController::connectToStation(const QString &address, int port, const QString &deviceId, const QString &platform)
{
    if (address.isEmpty() || port <= 0) {
        setStatus(QStringLiteral("Invalid station address"));
        return;
    }

    if (m_deviceId != deviceId || m_platform != platform) {
        m_deviceToken.clear();
    }

    m_address = address;
    m_port = port;
    m_deviceId = deviceId;
    m_platform = platform;

    m_socket.abort();

    if (!m_deviceId.isEmpty() && !m_platform.isEmpty() && m_deviceToken.isEmpty()) {
        requestDeviceToken();
        return;
    }

    openSocket();
}

void StationController::requestDeviceToken()
{
    Settings settings;
    const QString accessToken = settings.value(QStringLiteral("accessToken")).toString();
    if (accessToken.isEmpty()) {
        setStatus(QStringLiteral("Missing access token for station"));
        return;
    }

    QUrl url(QStringLiteral("https://quasar.yandex.net/glagol/token"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("device_id"), m_deviceId);
    query.addQueryItem(QStringLiteral("platform"), m_platform);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "OAuth " + accessToken.toUtf8());

    qDebug() << "StationController: requesting device token for" << m_deviceId << m_platform;
    setStatus(QStringLiteral("Requesting station token..."));

    QNetworkReply *reply = m_network.get(request);
    connect(reply, &QNetworkReply::finished, this, &StationController::onTokenReplyFinished);
}

void StationController::onTokenReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        return;
    }
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "StationController: token request error" << reply->errorString();
        setStatus(reply->errorString());
        return;
    }

    const QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    if (obj.value(QStringLiteral("status")).toString() != QStringLiteral("ok")) {
        setStatus(QStringLiteral("Failed to get station token"));
        return;
    }

    m_deviceToken = obj.value(QStringLiteral("token")).toString();
    if (m_deviceToken.isEmpty()) {
        setStatus(QStringLiteral("Empty station token"));
        return;
    }

    qDebug() << "StationController: got device token";
    openSocket();
}

void StationController::openSocket()
{
    QUrl url;
    url.setScheme(QStringLiteral("wss"));
    url.setHost(m_address);
    url.setPort(m_port);

    QSslConfiguration sslConfig = m_socket.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_socket.setSslConfiguration(sslConfig);

    qDebug() << "StationController: connecting to" << url;
    setStatus(QStringLiteral("Connecting to station..."));
    m_socket.open(url);
}

void StationController::disconnectFromStation()
{
    m_pendingPayload = QJsonObject();
    m_socket.close();
}

void StationController::sendPayload(const QJsonObject &payload)
{
    if (payload.isEmpty()) {
        return;
    }

    QString requestId = QUuid::createUuid().toString();
    requestId.remove(QLatin1Char('{'));
    requestId.remove(QLatin1Char('}'));

    QJsonObject message;
    message.insert(QStringLiteral("conversationToken"), m_deviceToken);
    message.insert(QStringLiteral("id"), requestId);
    message.insert(QStringLiteral("payload"), payload);
    message.insert(QStringLiteral("sentTime"), static_cast<double>(QDateTime::currentMSecsSinceEpoch()));

    const QString msg = QString::fromUtf8(QJsonDocument(message).toJson(QJsonDocument::Compact));
    if (!m_connected) {
        m_pendingPayload = payload;
        setStatus(QStringLiteral("Waiting for station connection..."));
        return;
    }

    qDebug() << "StationController: sending" << msg;
    m_socket.sendTextMessage(msg);
    setStatus(QStringLiteral("Command sent"));
}

void StationController::sendTextCommand(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    QJsonObject payload{
        {QStringLiteral("command"), QStringLiteral("sendText")},
        {QStringLiteral("text"), text}
    };
    sendPayload(payload);
}

void StationController::playTrackByIds(int trackId, int albumId)
{
    if (trackId <= 0) {
        return;
    }

    const QString trackIdStr = QString::number(trackId);
    const QString musicId = albumId > 0 ? (trackIdStr + QStringLiteral(":") + QString::number(albumId)) : trackIdStr;

    QJsonObject payload;
    payload.insert(QStringLiteral("command"), QStringLiteral("playMusic"));
    payload.insert(QStringLiteral("type"), QStringLiteral("track"));
    payload.insert(QStringLiteral("id"), musicId);

    sendPayload(payload);
}

void StationController::stopRemotePlayback()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("command"), QStringLiteral("stop"));
    sendPayload(payload);
}

void StationController::pauseRemotePlayback()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("command"), QStringLiteral("stop"));
    sendPayload(payload);
}

void StationController::resumeRemotePlayback()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("command"), QStringLiteral("play"));
    sendPayload(payload);
}

void StationController::setRemoteVolume(qreal volume)
{
    if (volume < 0.0) {
        volume = 0.0;
    }
    if (volume > 1.0) {
        volume = 1.0;
    }

    qDebug() << "StationController: set remote volume" << volume;
    QJsonObject payload;
    payload.insert(QStringLiteral("command"), QStringLiteral("setVolume"));
    payload.insert(QStringLiteral("volume"), volume);
    sendPayload(payload);
}

void StationController::onConnected()
{
    m_connected = true;
    emit connectedChanged();
    setStatus(QStringLiteral("Connected to station"));
    flushPendingCommand();
}

void StationController::flushPendingCommand()
{
    if (!m_connected || m_pendingPayload.isEmpty()) {
        return;
    }

    const QJsonObject payload = m_pendingPayload;
    m_pendingPayload = QJsonObject();
    sendPayload(payload);
}

void StationController::onDisconnected()
{
    if (m_connected) {
        m_connected = false;
        emit connectedChanged();
    }
    m_remotePlaying = false;
    m_remoteProgress = 0.0;
    m_remoteDuration = 0.0;
    m_remoteVolume = 0.0;
    const bool trackChanged = !m_remoteTrackId.isEmpty()
        || !m_remoteTrackTitle.isEmpty()
        || !m_remoteTrackArtist.isEmpty()
        || !m_remoteTrackCover.isEmpty();
    m_remoteTrackId.clear();
    m_remoteTrackTitle.clear();
    m_remoteTrackArtist.clear();
    m_remoteTrackCover.clear();
    emit remoteStateChanged();
    if (trackChanged) {
        emit remoteTrackChanged();
    }
    setStatus(QStringLiteral("Disconnected"));
}

void StationController::updateRemoteState(const QJsonObject &stateObj)
{
    bool changed = false;
    bool trackChanged = false;

    const bool playing = stateObj.value(QStringLiteral("playing")).toBool();
    if (m_remotePlaying != playing) {
        m_remotePlaying = playing;
        changed = true;
    }

    const QJsonObject playerState = stateObj.value(QStringLiteral("playerState")).toObject();
    const qreal progress = playerState.value(QStringLiteral("progress")).toDouble();
    const qreal duration = playerState.value(QStringLiteral("duration")).toDouble();
    const qreal volume = stateObj.value(QStringLiteral("volume")).toDouble(-1.0);
    const QString trackId = playerState.value(QStringLiteral("id")).toString();
    const QString title = playerState.value(QStringLiteral("title")).toString();
    const QString artist = playerState.value(QStringLiteral("subtitle")).toString();
    QString cover = playerState.value(QStringLiteral("extra")).toObject().value(QStringLiteral("coverURI")).toString();
    if (cover.startsWith(QStringLiteral("//"))) {
        cover.prepend(QStringLiteral("https:"));
    } else if (!cover.isEmpty() && !cover.contains(QStringLiteral("://"))) {
        cover.prepend(QStringLiteral("https://"));
    }
    cover.replace(QStringLiteral("%%"), QStringLiteral("400x400"));

    if (!qFuzzyCompare(m_remoteProgress + 1.0, progress + 1.0)) {
        m_remoteProgress = progress;
        changed = true;
    }
    if (!qFuzzyCompare(m_remoteDuration + 1.0, duration + 1.0)) {
        m_remoteDuration = duration;
        changed = true;
    }
    if (volume >= 0.0) {
        qDebug() << "StationController: station volume state" << volume << "current" << m_remoteVolume;
        if (!qFuzzyCompare(m_remoteVolume + 1.0, volume + 1.0)) {
            qDebug() << "StationController: remote volume changed" << volume;
            m_remoteVolume = volume;
            changed = true;
        }
    }
    if (!trackId.isEmpty() && m_remoteTrackId != trackId) {
        qDebug() << "StationController: remote track" << trackId;
        m_remoteTrackId = trackId;
        trackChanged = true;
    }
    if (m_remoteTrackTitle != title) {
        m_remoteTrackTitle = title;
        trackChanged = true;
    }
    if (m_remoteTrackArtist != artist) {
        m_remoteTrackArtist = artist;
        trackChanged = true;
    }
    if (m_remoteTrackCover != cover) {
        m_remoteTrackCover = cover;
        trackChanged = true;
    }

    if (changed) {
        emit remoteStateChanged();
    }
    if (trackChanged) {
        emit remoteTrackChanged();
    }
}

void StationController::onTextMessageReceived(const QString &message)
{
    qDebug() << "StationController: received" << message;
    const QJsonObject obj = QJsonDocument::fromJson(message.toUtf8()).object();
    if (obj.contains(QStringLiteral("state"))) {
        updateRemoteState(obj.value(QStringLiteral("state")).toObject());
    }
}

void StationController::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    qWarning() << "StationController: socket error" << m_socket.errorString();
    setStatus(m_socket.errorString());
}

void StationController::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors)
    m_socket.ignoreSslErrors();
}

void StationController::setStatus(const QString &status)
{
    if (m_status == status) {
        return;
    }
    m_status = status;
    emit statusChanged();
}
