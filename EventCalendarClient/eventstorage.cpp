#include "eventstorage.h"

EventStorage::EventStorage(QObject *parent) : QObject(parent) {}

void EventStorage::addEvent(const QDate &date, const QString &event) {
    if (date.isValid() && !event.isEmpty()) {
        _storage[date].insert(event);
    }
}

void EventStorage::clear() {
    _storage.clear();
}

QMap<QDate, QSet<QString>> EventStorage::getAllEvents() const {
    return _storage;
}

int EventStorage::totalCount() const {
    int total = 0;
    for (const auto &events : _storage) {
        total += events.size();
    }
    return total;
}
