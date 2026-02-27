#include "packetreceiver.h"
#include <QJsonDocument>
#include <QJsonObject>

PacketReceiver::PacketReceiver(QObject *parent) : QObject(parent) {
    _udpSocket = new QUdpSocket(this);
    _multicastAddress = QHostAddress("239.255.0.1");
    _receivePort = 45454;
    _active = false;
}

PacketReceiver::~PacketReceiver() {
    if (_active) {
        stop();
    }
}

bool PacketReceiver::start() {
    if (_active) return true;

    if (_udpSocket->bind(QHostAddress::AnyIPv4, _receivePort, QUdpSocket::ShareAddress)) {
        if (_udpSocket->joinMulticastGroup(_multicastAddress)) {
            _active = true;
            connect(_udpSocket, &QUdpSocket::readyRead, this, &PacketReceiver::processPendingDatagrams);
            return true;
        }
    }
    return false;
}

void PacketReceiver::stop() {
    if (!_active) return;

    _udpSocket->leaveMulticastGroup(_multicastAddress);
    _udpSocket->close();
    disconnect(_udpSocket, &QUdpSocket::readyRead, this, &PacketReceiver::processPendingDatagrams);
    _active = false;
}

void PacketReceiver::processPendingDatagrams() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        _udpSocket->readDatagram(datagram.data(), datagram.size());

        QDate date;
        QString event;

        if (parseEvent(datagram, date, event)) {
            emit eventReceived(date, event);
        }
    }
}

bool PacketReceiver::parseEvent(const QByteArray &data, QDate &date, QString &event) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    QJsonObject obj = doc.object();
    date = QDate::fromString(obj["date"].toString(), "yyyy-MM-dd");
    event = obj["event"].toString();

    return date.isValid() && !event.isEmpty();
}

bool PacketReceiver::isActive() const {
    return _active;
}
