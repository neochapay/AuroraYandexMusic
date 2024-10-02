#include "stantiondetector.h"
#include <qmdnsengine/resolver.h>

#include <QDebug>

StantionDetector::StantionDetector(QObject *parent)
    : QObject(parent)
    , m_DetectorCache(new QMdnsEngine::Cache())
    , m_DetrctorServer(new QMdnsEngine::Server())
    , m_DetrctorBrowser(new QMdnsEngine::Browser(m_DetrctorServer, "_yandexio._tcp.local."))
    , m_stantionAddress("")
    , m_stantionName("")
    , m_stantionPort(0)
    , m_stantionPlatform("")
{
    m_stantions["yandexstation"]   = "Яндекс Станция (2018)";
    m_stantions["yandexstation_2"] = "Яндекс Станция Макс (2020)";
    m_stantions["yandexmini"]      = "Яндекс Станция Мини (2019)";
    m_stantions["yandexmini_2"]    = "Яндекс Станция Мини 2 (2021)";
    m_stantions["yandexmicro"]     = "Яндекс Станция Лайт (2021)";
    m_stantions["yandexmidi"]      = "Яндекс Станция 2 (2022)";
    m_stantions["cucumber"]        = "Яндекс Станция Миди (2023)";
    m_stantions["chiron"]          = "Яндекс Станция Дуо Макс (2023)";
    m_stantions["yandexmodule"]    = "Яндекс Модуль (2019)";
    m_stantions["yandexmodule_2"]  = "Яндекс Модуль 2 (2021)";
    m_stantions["yandex_tv"]       = "ТВ с Алисой";
    m_stantions["goya"]            = "Яндекс ТВ (2022)";
    m_stantions["magritte"]        = "Яндекс ТВ Станция (2023)";

    connect(m_DetrctorBrowser, &QMdnsEngine::Browser::serviceAdded, this, &StantionDetector::onServiceAdded);
}

StantionDetector::~StantionDetector()
{
    delete m_DetectorCache;
    delete m_DetrctorServer;
    delete m_DetrctorBrowser;
}

void StantionDetector::onServiceAdded(const QMdnsEngine::Service &service)
{
    m_stantionPlatform = service.attributes()["platform"];
    if(m_stantionPlatform.isEmpty()) {
        return;
    }

    if(!isYandexStantion(m_stantionPlatform)) {
        return;
    }

    QMdnsEngine::Resolver *resolver = new QMdnsEngine::Resolver(m_DetrctorServer, service.hostname(), nullptr, this);
    connect(resolver, &QMdnsEngine::Resolver::resolved, [this](const QHostAddress &address) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            if(address.toString() != m_stantionAddress) {
                m_stantionAddress = address.toString();
                emit stantionChanged();
            }
        }
    });

    if(m_stantionName != m_stantions.value(m_stantionPlatform) || m_stantionPort != service.port()) {
        m_stantionName = m_stantions.value(m_stantionPlatform);
        m_stantionPort = service.port();
        emit stantionChanged();
    }
}

bool StantionDetector::isYandexStantion(const QString platform)
{
    if(m_stantions.value(platform).isEmpty()) {
        return false;
    }
    return true;
}


const QString &StantionDetector::stantionAddress() const
{
    return m_stantionAddress;
}

const QString &StantionDetector::stantionName() const
{
    return m_stantionName;
}

int StantionDetector::stantionPort() const
{
    return m_stantionPort;
}

bool StantionDetector::valid() const
{
    return m_stantionPort > 0 && !m_stantionAddress.isEmpty() && !m_stantionName.isEmpty();
}

const QString &StantionDetector::stantionPlatform() const
{
    return m_stantionPlatform;
}
