#ifndef NETWORK_H
#define NETWORK_H

#include "packetsender.h"
#include "packetreceiver.h"

#include <QObject>

class Network : public QObject {
    Q_OBJECT

public:
    explicit Network(QObject *parent = nullptr);

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
