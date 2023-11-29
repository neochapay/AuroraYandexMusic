#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID NOTIFY userIDChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)

    int m_userID;

    QString m_displayName;

public:
    explicit User(QObject* parent = nullptr);
    Q_INVOKABLE void getAccountStatus();
    Q_INVOKABLE void getFeed();

    int userID() const;
    const QString& displayName() const;

private slots:
    void getAccountStatusHandler(QJsonValue& value);
    void getFeedHandler(QJsonValue& value);

signals:
    void userIDChanged();
    void displayNameChanged();
};

#endif // USER_H
