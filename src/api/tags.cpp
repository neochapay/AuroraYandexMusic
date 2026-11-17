#include "tags.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>

Tags::Tags(QObject *parent) : QObject(parent)
{
}

void Tags::getTag(const QString &tagId)
{
    getPlaylistsIdsByTag(tagId);
}

void Tags::getPlaylistsIdsByTag(const QString &tagId)
{
    if (tagId.trimmed().isEmpty()) {
        return;
    }
    m_tagId = tagId.trimmed();

    Request* request = new Request("/tags/" + m_tagId + "/playlist-ids");
    connect(request, &Request::dataReady, this, &Tags::tagRequestHandler);
    request->get();
}

void Tags::tagRequestHandler(QJsonValue value)
{
    QJsonObject result = value.toObject();
    QJsonObject tag = result.value(QStringLiteral("tag")).toObject();
    QJsonArray ids = result.value(QStringLiteral("ids")).toArray();

    QStringList idStrings;
    for (const QJsonValue &v : ids) {
        if (v.isObject()) {
            const QJsonObject o = v.toObject();
            const int uid = o.value(QStringLiteral("uid")).toInt();
            const int kind = o.value(QStringLiteral("kind")).toInt();
            if (uid > 0 && kind > 0) {
                idStrings << QString("%1:%2").arg(uid).arg(kind);
            }
        } else if (v.isString()) {
            idStrings << v.toString();
        }
    }

    emit tagReady(m_tagId, tag, ids, idStrings);
    emit tagResultReady(m_tagId, result);
}
