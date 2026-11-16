#include "booksandpodcasts.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>

BooksAndPodcasts::BooksAndPodcasts(QObject *parent) : QObject(parent)
{
}

void BooksAndPodcasts::getBooksAndPodcasts()
{
    getCatalog();
}

void BooksAndPodcasts::getCatalog()
{
    // Endpoint path as defined in the OpenAPI spec
    Request* request = new Request("/non-music/calague");
    connect(request, &Request::dataReady, this, &BooksAndPodcasts::catalogRequestHandler);
    request->get();
}

void BooksAndPodcasts::catalogRequestHandler(QJsonValue value)
{
    QJsonObject result = value.toObject();
    emit catalogReady(result);
    emit blocksReady(result.value(QStringLiteral("title")).toString(),
                     result.value(QStringLiteral("blocks")).toArray());
}
