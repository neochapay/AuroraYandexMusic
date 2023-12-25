#ifndef LANDINGRESULTBLOCK_H
#define LANDINGRESULTBLOCK_H

#include <QObject>

class LandingResultBlock : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString type MEMBER type)
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString description MEMBER description)
    Q_PROPERTY(QList<QObject*> entities MEMBER entities)

public:
    QString type;
    QString title;
    QString description;
    QList<QObject*> entities;
};


#endif // LANDINGRESULTBLOCK_H
