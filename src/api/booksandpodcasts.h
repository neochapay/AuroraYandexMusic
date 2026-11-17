#ifndef BOOKSANDPODCASTS_H
#define BOOKSANDPODCASTS_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

class BooksAndPodcasts : public QObject
{
    Q_OBJECT
public:
    explicit BooksAndPodcasts(QObject *parent = nullptr);

    Q_INVOKABLE void getCatalog();
    Q_INVOKABLE void getBooksAndPodcasts(); // alias

signals:
    void catalogReady(const QJsonObject &result);
    void blocksReady(const QString &title, const QJsonArray &blocks);
    void error(const QString &message);

private slots:
    void catalogRequestHandler(QJsonValue value);
};

#endif // BOOKSANDPODCASTS_H
