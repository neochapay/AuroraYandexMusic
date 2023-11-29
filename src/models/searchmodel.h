#ifndef SearchModel_H
#define SearchModel_H

#include "playlistmodel.h"
#include <QAbstractListModel>
#include <QJsonValue>
#include <QObject>

#include "../apirequest.h"
#include "../trackobject.h"

class SearchModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit SearchModel(QObject* parent = 0);
    virtual ~SearchModel() {};

    //   static SearchModel *model;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_hash; }

    bool insertRows(int position, int rows, TrackObject* item, const QModelIndex& index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());

    Q_INVOKABLE void searchTracks(QString q);
    Q_INVOKABLE void playTrack();
    Q_INVOKABLE void sendFeedback(QString type);
    Q_INVOKABLE void setCurrentIndex(int currentIndex);
    int currentIndex() { return m_currentIndex; }
    QString currentSong() { return m_currentSong; }
    QString currentArtist() { return m_currentArtist; }
    bool m_loading;
    QList<TrackObject*> m_playList;
    Q_INVOKABLE QList<TrackObject*> playlist();

signals:
    void loadFirstDataFinished();
    void currentIndexChanged(int currentIndex);

public slots:
    QVariant get(const int idx);

private slots:
    void getSearchTracksFinished(const QJsonValue& value);

private:
    int m_currentIndex;
    QString batchid;
    QString m_currentSong;
    QString m_currentArtist;

    QHash<int, QByteArray> m_hash;
    ApiRequest* m_api;
    QJsonValue m_oldValue;
};

#endif // SearchModel_H
