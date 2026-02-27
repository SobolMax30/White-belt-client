#ifndef EVENTSTORAGE_H
#define EVENTSTORAGE_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QSet>

class EventStorage : public QObject {
    Q_OBJECT

public:
    EventStorage(QObject *parent = nullptr);

    void addEvent(const QDate &date, const QString &event);
    QMap<QDate, QSet<QString>> getAllEvents() const;
    int totalCount() const;
    void clear();

private:
    QMap<QDate, QSet<QString>> _storage;
};

#endif // EVENTSTORAGE_H
