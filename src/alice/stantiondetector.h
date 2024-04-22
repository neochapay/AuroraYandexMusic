#ifndef STATIONDETECTOR_H
#define STATIONDETECTOR_H

#include <QObject>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/browser.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

class StantionDetector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString stantionAddress READ stantionAddress NOTIFY stantionChanged)
    Q_PROPERTY(QString stantionName READ stantionName NOTIFY stantionChanged)
    Q_PROPERTY(QString stantionPlatform READ stantionPlatform NOTIFY stantionChanged)
    Q_PROPERTY(int stantionPort READ stantionPort NOTIFY stantionChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY stantionChanged)

public:
    explicit StantionDetector(QObject *parent = nullptr);
    const QString &stantionAddress() const;
    const QString &stantionName() const;
    int stantionPort() const;
    bool valid() const;
    const QString &stantionPlatform() const;

signals:
    void stantionChanged();

private slots:
    void onServiceAdded(const QMdnsEngine::Service &service);

private:
    bool isYandexStantion(const QString platform);

    QMdnsEngine::Cache m_DetectorCache;
    QMdnsEngine::Server *m_DetrctorServer;
    QMdnsEngine::Browser* m_DetrctorBrowser;
    QString m_stantionAddress;

    QMap<QString, QString> m_stantions;
    QString m_stantionName;
    int m_stantionPort;
    QString m_stantionPlatform;
};

#endif // STATIONDETECTOR_H
