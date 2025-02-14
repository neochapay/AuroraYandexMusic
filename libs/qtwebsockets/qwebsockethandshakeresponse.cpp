/****************************************************************************
**
** Copyright (C) 2014 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qwebsockethandshakerequest_p.h"
#include "qwebsockethandshakeresponse_p.h"
#include "qwebsocketprotocol.h"
#include "qwebsocketprotocol_p.h"

#include <QtCore/QByteArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QStringBuilder> //for more efficient string concatenation
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#include <functional> //for std::greater
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <algorithm>
#include <iterator>
#endif

QT_BEGIN_NAMESPACE

/*!
    \internal
 */
QWebSocketHandshakeResponse::QWebSocketHandshakeResponse(
    const QWebSocketHandshakeRequest& request,
    const QString& serverName,
    bool isOriginAllowed,
    const QList<QWebSocketProtocol::Version>& supportedVersions,
    const QList<QString>& supportedProtocols,
    const QList<QString>& supportedExtensions)
    : m_isValid(false)
    , m_canUpgrade(false)
    , m_response()
    , m_acceptedProtocol()
    , m_acceptedExtension()
    , m_acceptedVersion(QWebSocketProtocol::VersionUnknown)
    , m_error(QWebSocketProtocol::CloseCodeNormal)
    , m_errorString()
{
    m_response = getHandshakeResponse(request, serverName,
        isOriginAllowed, supportedVersions,
        supportedProtocols, supportedExtensions);
    m_isValid = true;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0 )
template <class T, class Compare>
static QList<T> listIntersection(QList<T> list1, QList<T> list2, Compare comp)
{
    QList<T> result;
    std::sort(list1.begin(), list1.end(), comp);
    std::sort(list2.begin(), list2.end(), comp);
    std::set_intersection(list1.cbegin(), list1.cend(),
                          list2.cbegin(), list2.cend(),
                          std::back_inserter(result), comp);
    return result;
}
#endif

/*!
    \internal
 */
QWebSocketHandshakeResponse::~QWebSocketHandshakeResponse()
{
}

/*!
    \internal
 */
bool QWebSocketHandshakeResponse::isValid() const
{
    return m_isValid;
}

/*!
    \internal
 */
bool QWebSocketHandshakeResponse::canUpgrade() const
{
    return m_isValid && m_canUpgrade;
}

/*!
    \internal
 */
QString QWebSocketHandshakeResponse::acceptedProtocol() const
{
    return m_acceptedProtocol;
}

/*!
    \internal
 */
QString QWebSocketHandshakeResponse::calculateAcceptKey(const QString& key) const
{
    // the UID comes from RFC6455
    const QString tmpKey = key % QStringLiteral("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
    const QByteArray hash = QCryptographicHash::hash(tmpKey.toLatin1(), QCryptographicHash::Sha1);
    return QString::fromLatin1(hash.toBase64());
}

/*!
    \internal
 */
QString QWebSocketHandshakeResponse::getHandshakeResponse(
    const QWebSocketHandshakeRequest& request,
    const QString& serverName,
    bool isOriginAllowed,
    const QList<QWebSocketProtocol::Version>& supportedVersions,
    const QList<QString>& supportedProtocols,
    const QList<QString>& supportedExtensions)
{
    QStringList response;
    m_canUpgrade = false;

    if (!isOriginAllowed) {
        if (!m_canUpgrade) {
            m_error = QWebSocketProtocol::CloseCodePolicyViolated;
            m_errorString = tr("Access forbidden.");
            response << QStringLiteral("HTTP/1.1 403 Access Forbidden");
        }
    } else {
        if (request.isValid()) {
            const QString acceptKey = calculateAcceptKey(request.key());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0 )
            const QList<QString> matchingProtocols = listIntersection(supportedProtocols, request.protocols(), std::less<>());
#else
            const QList<QString> matchingProtocols = supportedProtocols.toSet().intersect(request.protocols().toSet()).toList();
#endif
            // TODO: extensions must be kept in the order in which they arrive
            // cannot use set.intersect() to get the supported extensions
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0 )
            const QList<QString> matchingExtensions = QSet<QString>(supportedExtensions.begin(), supportedExtensions.end())
                                                          .intersect(QSet<QString>(request.extensions().begin(),request.extensions().end())).values();
            QList<QWebSocketProtocol::Version> matchingVersions = listIntersection(supportedVersions, request.versions(), std::less<>());
#else
            const QList<QString> matchingExtensions = supportedExtensions.toSet().intersect(request.extensions().toSet()).toList();
            QList<QWebSocketProtocol::Version> matchingVersions = request.versions().toSet().intersect(supportedVersions.toSet()).toList();
#endif
            std::sort(matchingVersions.begin(), matchingVersions.end(),
                std::greater<QWebSocketProtocol::Version>()); // sort in descending order

            if (Q_UNLIKELY(matchingVersions.isEmpty())) {
                m_error = QWebSocketProtocol::CloseCodeProtocolError;
                m_errorString = tr("Unsupported version requested.");
                m_canUpgrade = false;
            } else {
                response << QStringLiteral("HTTP/1.1 101 Switching Protocols") << QStringLiteral("Upgrade: websocket") << QStringLiteral("Connection: Upgrade") << QStringLiteral("Sec-WebSocket-Accept: ") % acceptKey;
                if (!matchingProtocols.isEmpty()) {
                    m_acceptedProtocol = matchingProtocols.first();
                    response << QStringLiteral("Sec-WebSocket-Protocol: ") % m_acceptedProtocol;
                }
                if (!matchingExtensions.isEmpty()) {
                    m_acceptedExtension = matchingExtensions.first();
                    response << QStringLiteral("Sec-WebSocket-Extensions: ") % m_acceptedExtension;
                }
                QString origin = request.origin().trimmed();
                if (origin.contains(QStringLiteral("\r\n")) || serverName.contains(QStringLiteral("\r\n"))) {
                    m_error = QWebSocketProtocol::CloseCodeAbnormalDisconnection;
                    m_errorString = tr("One of the headers contains a newline. "
                                       "Possible attack detected.");
                    m_canUpgrade = false;
                } else {
                    if (origin.isEmpty())
                        origin = QStringLiteral("*");
                    QDateTime datetime = QDateTime::currentDateTimeUtc();
                    response << QStringLiteral("Server: ") % serverName << QStringLiteral("Access-Control-Allow-Credentials: false") << QStringLiteral("Access-Control-Allow-Methods: GET") << QStringLiteral("Access-Control-Allow-Headers: content-type") << QStringLiteral("Access-Control-Allow-Origin: ") % origin << QStringLiteral("Date: ") % QLocale::c().toString(datetime, QStringLiteral("ddd, dd MMM yyyy hh:mm:ss 'GMT'"));

                    m_acceptedVersion = QWebSocketProtocol::currentVersion();
                    m_canUpgrade = true;
                }
            }
        } else {
            m_error = QWebSocketProtocol::CloseCodeProtocolError;
            m_errorString = tr("Bad handshake request received.");
            m_canUpgrade = false;
        }
        if (Q_UNLIKELY(!m_canUpgrade)) {
            response << QStringLiteral("HTTP/1.1 400 Bad Request");
            QStringList versions;
            Q_FOREACH (QWebSocketProtocol::Version version, supportedVersions)
                versions << QString::number(static_cast<int>(version));
            response << QStringLiteral("Sec-WebSocket-Version: ")
                    % versions.join(QStringLiteral(", "));
        }
    }
    response << QStringLiteral("\r\n"); // append empty line at end of header
    return response.join(QStringLiteral("\r\n"));
}

/*!
    \internal
 */
QTextStream& QWebSocketHandshakeResponse::writeToStream(QTextStream& textStream) const
{
    if (Q_LIKELY(!m_response.isEmpty()))
        textStream << m_response.toLatin1().constData();
    else
        textStream.setStatus(QTextStream::WriteFailed);
    return textStream;
}

/*!
    \internal
 */
QTextStream& operator<<(QTextStream& stream, const QWebSocketHandshakeResponse& response)
{
    return response.writeToStream(stream);
}

/*!
    \internal
 */
QWebSocketProtocol::Version QWebSocketHandshakeResponse::acceptedVersion() const
{
    return m_acceptedVersion;
}

/*!
    \internal
 */
QWebSocketProtocol::CloseCode QWebSocketHandshakeResponse::error() const
{
    return m_error;
}

/*!
    \internal
 */
QString QWebSocketHandshakeResponse::errorString() const
{
    return m_errorString;
}

/*!
    \internal
 */
QString QWebSocketHandshakeResponse::acceptedExtension() const
{
    return m_acceptedExtension;
}

QT_END_NAMESPACE
