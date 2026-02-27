#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include "packetsender.h"
#include "packetreceiver.h"

class Network : public QObject {
    Q_OBJECT

public:
    Network(QObject *parent = nullptr);

    bool startReceiver();
    void stopReceiver();
    void requestSync();
    void setEnabled(bool enabled);
    bool isConnected() const;

signals:
    void eventReceived(const QDate &date, const QString &event);

private:
    PacketSender *_sender;
    PacketReceiver *_receiver;
};

#endif // NETWORK_H
