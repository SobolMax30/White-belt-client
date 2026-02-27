#include "network.h"

Network::Network(QObject *parent) : QObject(parent) {
    _sender = new PacketSender(this);
    _receiver = new PacketReceiver(this);

    connect(_receiver, &PacketReceiver::eventReceived, this, &Network::eventReceived);
}

bool Network::startReceiver() {
    return _receiver->start();
}

void Network::stopReceiver() {
    _receiver->stop();
}

void Network::requestSync() {
    _sender->sendSyncRequest();
}

void Network::setEnabled(bool enabled) {
    _sender->setEnabled(enabled);
}

bool Network::isConnected() const {
    return _receiver->isActive();
}
