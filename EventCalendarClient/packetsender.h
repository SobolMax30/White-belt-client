#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class PacketSender : public QObject {
    Q_OBJECT

public:
    explicit PacketSender(QObject *parent = nullptr);

    void sendSyncRequest();
    void setEnabled(bool enabled);

private:
    QUdpSocket *_udpSocket;
    QHostAddress _multicastAddress;
    quint16 _syncPort;
    bool _enabled;
};

#endif // PACKETSENDER_H
