/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef QMDNSENGINE_PROBER_P_H
#define QMDNSENGINE_PROBER_P_H

#include <QObject>
#include <QTimer>

#include "record.h"

namespace QMdnsEngine
{

class AbstractServer;
class Message;
class Prober;

class ProberPrivate : public QObject
{
    Q_OBJECT

public:

    ProberPrivate(Prober *prober, AbstractServer *server, const Record &record);

    void assertRecord();

    AbstractServer *server;
    QTimer timer;

    bool confirmed;

    Record proposedRecord;
    QByteArray name;
    QByteArray type;
    int suffix;

private Q_SLOTS:

    void onMessageReceived(const Message &message);
    void onTimeout();

private:

    Prober *const q;
};

}

#endif // QMDNSENGINE_PROBER_P_H
