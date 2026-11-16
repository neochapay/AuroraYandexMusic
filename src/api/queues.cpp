#include "queues.h"
#include "request.h"

#include <QJsonObject>
#include <QUrlQuery>

Queues::Queues(QObject *parent) : QObject(parent)
{
    // Default device header as used by common clients; can be overridden
    m_deviceHeader = QStringLiteral("os=unknown; os_version=unknown; manufacturer=unknown; model=unknown; clid=; device_id=unknown; uuid=unknown");
}

QString Queues::deviceHeader() const
{
    return m_deviceHeader;
}

void Queues::setDeviceHeader(const QString &deviceHeader)
{
    if (m_deviceHeader == deviceHeader)
        return;
    m_deviceHeader = deviceHeader;
    emit deviceHeaderChanged();
}

void Queues::applyDeviceHeader(Request *request, const QString &deviceHeader) const
{
    const QString header = deviceHeader.isEmpty() ? m_deviceHeader : deviceHeader;
    if (!header.isEmpty()) {
        request->setRawHeader("X-Yandex-Music-Device", header.toUtf8());
    }
}

void Queues::getQueues(const QString &deviceHeader)
{
    Request* request = new Request("/queues");
    applyDeviceHeader(request, deviceHeader);
    connect(request, &Request::dataReady, this, &Queues::getQueuesRequestHandler);
    request->get();
}

void Queues::getQueueById(const QString &queueId)
{
    if (queueId.isEmpty()) {
        return;
    }
    m_lastQueueId = queueId;
    Request* request = new Request("/queues/" + queueId);
    connect(request, &Request::dataReady, this, &Queues::getQueueByIdRequestHandler);
    request->get();
}

void Queues::updateQueuePosition(const QString &queueId, int currentIndex, bool isInteractive)
{
    if (queueId.isEmpty()) {
        return;
    }
    m_lastQueueId = queueId;

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("currentIndex"), QString::number(currentIndex));
    query.addQueryItem(QStringLiteral("IsInteractive"), isInteractive ? QStringLiteral("true") : QStringLiteral("false"));

    Request* request = new Request("/queues/" + queueId + "/update-position?" + query.toString(QUrl::FullyEncoded));
    connect(request, &Request::dataReady, this, &Queues::updateQueuePositionRequestHandler);
    request->post();
}

void Queues::getQueuesRequestHandler(QJsonValue value)
{
    emit queuesReady(value);
}

void Queues::getQueueByIdRequestHandler(QJsonValue value)
{
    QJsonObject obj;
    if (value.isObject()) {
        obj = value.toObject();
    }
    emit queueReady(m_lastQueueId, obj);
}

void Queues::updateQueuePositionRequestHandler(QJsonValue value)
{
    emit queuePositionUpdated(m_lastQueueId, value);
}
