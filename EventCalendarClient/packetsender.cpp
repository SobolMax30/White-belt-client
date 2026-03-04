#include "packetsender.h"

#include <QDebug>

PacketSender::PacketSender(QObject *parent) : QObject(parent) {
    _udpSocket = new QUdpSocket(this);
    _multicastAddress = QHostAddress("239.255.0.1");
    _syncPort = 45455;
    _enabled = false;
}

void PacketSender::sendSyncRequest() {
    if (!_enabled) {
        return;
    }

    QByteArray data = "SYNC_REQUEST";
    qint64 sent = _udpSocket->writeDatagram(data, _multicastAddress, _syncPort);

    if (sent != -1) {
        qDebug() << "Sync request sent";
    } else {
        qDebug() << "Failed to send sync request:" << _udpSocket->errorString();
    }
}

void PacketSender::setEnabled(bool enabled) {
    _enabled = enabled;
}
