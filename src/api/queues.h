#ifndef QUEUES_H
#define QUEUES_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QString>

class Queues : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceHeader READ deviceHeader WRITE setDeviceHeader NOTIFY deviceHeaderChanged)

public:
    explicit Queues(QObject *parent = nullptr);

    QString deviceHeader() const;
    void setDeviceHeader(const QString &deviceHeader);

    Q_INVOKABLE void getQueues(const QString &deviceHeader = QString());
    Q_INVOKABLE void getQueueById(const QString &queueId);
    Q_INVOKABLE void updateQueuePosition(const QString &queueId, int currentIndex, bool isInteractive = true);

signals:
    void deviceHeaderChanged();
    void queuesReady(const QJsonValue &queuesResult);
    void queueReady(const QString &queueId, const QJsonObject &queueItem);
    void queuePositionUpdated(const QString &queueId, const QJsonValue &result);
    void error(const QString &message);

private slots:
    void getQueuesRequestHandler(QJsonValue value);
    void getQueueByIdRequestHandler(QJsonValue value);
    void updateQueuePositionRequestHandler(QJsonValue value);

private:
    void applyDeviceHeader(class Request *request, const QString &deviceHeader) const;

    QString m_deviceHeader;
    QString m_lastQueueId;
};

#endif // QUEUES_H
