#ifndef STATIONCONTROLLER_H
#define STATIONCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QJsonObject>
#include <qwebsocket.h>

class StationController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool remotePlaying READ remotePlaying NOTIFY remoteStateChanged)
    Q_PROPERTY(qreal remoteProgress READ remoteProgress NOTIFY remoteStateChanged)
    Q_PROPERTY(qreal remoteDuration READ remoteDuration NOTIFY remoteStateChanged)
    Q_PROPERTY(qreal remoteVolume READ remoteVolume NOTIFY remoteStateChanged)
    Q_PROPERTY(QString remoteTrackId READ remoteTrackId NOTIFY remoteTrackChanged)
    Q_PROPERTY(QString remoteTrackTitle READ remoteTrackTitle NOTIFY remoteTrackChanged)
    Q_PROPERTY(QString remoteTrackArtist READ remoteTrackArtist NOTIFY remoteTrackChanged)
    Q_PROPERTY(QString remoteTrackCover READ remoteTrackCover NOTIFY remoteTrackChanged)

public:
    explicit StationController(QObject *parent = nullptr);
    ~StationController() override;

    bool connected() const;
    QString status() const;
    bool remotePlaying() const;
    qreal remoteProgress() const;
    qreal remoteDuration() const;
    qreal remoteVolume() const;
    QString remoteTrackId() const;
    QString remoteTrackTitle() const;
    QString remoteTrackArtist() const;
    QString remoteTrackCover() const;

    Q_INVOKABLE void connectToStation(const QString &address, int port, const QString &deviceId = QString(), const QString &platform = QString());
    Q_INVOKABLE void disconnectFromStation();
    Q_INVOKABLE void sendTextCommand(const QString &text);
    Q_INVOKABLE void playTrackByIds(int trackId, int albumId);
    Q_INVOKABLE void stopRemotePlayback();
    Q_INVOKABLE void pauseRemotePlayback();
    Q_INVOKABLE void resumeRemotePlayback();
    Q_INVOKABLE void setRemoteVolume(qreal volume);

signals:
    void connectedChanged();
    void statusChanged();
    void remoteStateChanged();
    void remoteTrackChanged();

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void onTokenReplyFinished();

private:
    void setStatus(const QString &status);
    void requestDeviceToken();
    void openSocket();
    void sendPayload(const QJsonObject &payload);
    void flushPendingCommand();
    void updateRemoteState(const QJsonObject &stateObj);

    QWebSocket m_socket;
    QNetworkAccessManager m_network;
    bool m_connected = false;
    QString m_status;
    QString m_address;
    int m_port = 0;
    QString m_deviceId;
    QString m_platform;
    QString m_deviceToken;
    QJsonObject m_pendingPayload;
    bool m_remotePlaying = false;
    qreal m_remoteProgress = 0.0;
    qreal m_remoteDuration = 0.0;
    qreal m_remoteVolume = 0.0;
    QString m_remoteTrackId;
    QString m_remoteTrackTitle;
    QString m_remoteTrackArtist;
    QString m_remoteTrackCover;
};

#endif // STATIONCONTROLLER_H
