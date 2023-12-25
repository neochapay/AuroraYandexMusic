#ifndef SEARCHRESULTDATA_H
#define SEARCHRESULTDATA_H

#include <QObject>

class SearchResultData : public QObject{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> artists MEMBER artists)
    Q_PROPERTY(QList<QObject*> albums MEMBER albums)
    Q_PROPERTY(QList<QObject*> tracks MEMBER tracks)

public:
    QList<QObject*> artists;
    QList<QObject*> albums;
    QList<QObject*> tracks;
};


#endif // SEARCHRESULTDATA_H
