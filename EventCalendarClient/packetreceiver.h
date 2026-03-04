#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDate>

class PacketReceiver : public QObject {
    Q_OBJECT

public:
    explicit PacketReceiver(QObject *parent = nullptr);
    ~PacketReceiver();

    bool start();
    void stop();
    bool isActive() const;

signals:
    void eventReceived(const QDate &date, const QString &event);

private slots:
    void processPendingDatagrams();

private:
    bool parseEvent(const QByteArray &data, QDate &date, QString &event);

    QUdpSocket *_udpSocket;
    QHostAddress _multicastAddress;
    quint16 _receivePort;
    bool _active;
};

#endif // PACKETRECEIVER_H
