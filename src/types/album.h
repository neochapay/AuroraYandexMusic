#ifndef ALBUH
#define ALBUH

#include "artist.h"
#include <QDateTime>
#include <QObject>

struct Label {
    Q_GADGET
public:
    int id;
    QString name;
};

class AlbumPrivate {
public:
    QList<Artist> artists;
    bool available;
    bool availableForMobile;
    QStringList availableForOptions;
    bool availableForPremiumUsers;
    bool availablePartially;
    QList<int> bests;
    QString contentWarning;
    QString coverUri;
    QStringList disclaimers;
    QString genere;
    int albumId;
    QList<Label> labels;
    int likesConut;
    QString metaType;
    QString orImage;
    bool recent;
    QDateTime releaseDate;
    QString title;
    int trackCount;
    int trackPositionIndex;
    int trackPositionVolume;
    int year;
};

class Album : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<Artist> artists READ artists)
    Q_PROPERTY(bool available READ available)
    Q_PROPERTY(bool availableForMobile READ availableForMobile)
    Q_PROPERTY(QStringList availableForOptions READ availableForOptions)
    Q_PROPERTY(bool availableForPremiumUsers READ availableForPremiumUsers)
    Q_PROPERTY(bool availablePartially READ availablePartially)
    Q_PROPERTY(QList<int> bests READ bests)
    Q_PROPERTY(QString contentWarning READ contentWarning)
    Q_PROPERTY(QString coverUri READ coverUri)
    Q_PROPERTY(QStringList disclaimers READ disclaimers)
    Q_PROPERTY(QString genere READ genere)
    Q_PROPERTY(int albumId READ albumId)
    Q_PROPERTY(QList<Label> labels READ labels)
    Q_PROPERTY(int likesConut READ likesConut)
    Q_PROPERTY(QString metaType READ metaType)
    Q_PROPERTY(QString orImage READ orImage)
    Q_PROPERTY(bool recent READ recent)
    Q_PROPERTY(QDateTime releaseDate READ releaseDate)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(int trackCount READ trackCount)
    Q_PROPERTY(int trackPositionIndex READ trackPositionIndex)
    Q_PROPERTY(int trackPositionVolume READ trackPositionVolume)
    Q_PROPERTY(int year READ year)

public:
    explicit Album(QObject* parent = nullptr);
    explicit Album(QJsonObject object, QObject* parent = nullptr);
    virtual ~Album();
    Album& operator=(const Album& other);

    const QList<Artist>& artists() const;
    bool available() const;
    bool availableForMobile() const;
    const QStringList& availableForOptions() const;
    bool availableForPremiumUsers() const;
    bool availablePartially() const;
    const QList<int>& bests() const;
    const QString& contentWarning() const;
    const QString& coverUri() const;
    const QStringList& disclaimers() const;
    const QString& genere() const;
    int albumId() const;
    const QList<Label> labels() const;
    int likesConut() const;
    const QString& metaType() const;
    const QString& orImage() const;
    bool recent() const;
    const QDateTime& releaseDate() const;
    const QString& title() const;
    int trackCount() const;
    int trackPositionIndex() const;
    int trackPositionVolume() const;
    int year() const;

private:
    AlbumPrivate* d_ptr;
};

#endif // ALBUH
