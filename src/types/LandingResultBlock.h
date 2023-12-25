#ifndef LANDINGRESULTBLOCK_H
#define LANDINGRESULTBLOCK_H

#include <QObject>

class LandingResultBlock : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString type MEMBER type NOTIFY landingResultBlockChanged)
    Q_PROPERTY(QString title MEMBER title NOTIFY landingResultBlockChanged)
    Q_PROPERTY(QString description MEMBER description NOTIFY landingResultBlockChanged)
    Q_PROPERTY(QList<QObject*> entities MEMBER entities NOTIFY landingResultBlockChanged)

public:
    QString type;
    QString title;
    QString description;
    QList<QObject*> entities;

signals:
    void landingResultBlockChanged();
};


#endif // LANDINGRESULTBLOCK_H
