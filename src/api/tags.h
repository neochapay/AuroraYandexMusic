#ifndef TAGS_H
#define TAGS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QStringList>

class Tags : public QObject
{
    Q_OBJECT
public:
    explicit Tags(QObject *parent = nullptr);

    Q_INVOKABLE void getPlaylistsIdsByTag(const QString &tagId);
    Q_INVOKABLE void getTag(const QString &tagId); // alias

signals:
    void tagReady(const QString &tagId, const QJsonObject &tag, const QJsonArray &playlistIds, const QStringList &playlistIdStrings);
    void tagResultReady(const QString &tagId, const QJsonObject &result);
    void error(const QString &message);

private slots:
    void tagRequestHandler(QJsonValue value);

private:
    QString m_tagId;
};

#endif // TAGS_H
