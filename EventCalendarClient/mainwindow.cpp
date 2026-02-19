#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <cstring>

QString _baseStyle = "border-radius: 10px; border: 1px solid black;";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->statusLabel->setFixedSize(20, 20);
    ui->statusLabel->setStyleSheet(_baseStyle + "background-color: red;");

    _udpSocket = new QUdpSocket(this);
    _multicastAddress = QHostAddress("239.255.0.1");
    _multicastPort = 45454;
    _connected = false;

    QString addressString = _multicastAddress.toString();
    QString portString = QString::number(_multicastPort);

    ui->treeWidget->setHeaderLabel("Полученные события");
    ui->label_Address->setText("Адрес: " + addressString);
    ui->label_Port->setText("Порт: " + portString);
    ui->label_Count->setText("Всего событий: 0");
}

MainWindow::~MainWindow() {
    if (_connected) {
        _udpSocket->leaveMulticastGroup(_multicastAddress);
    }

    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if (!_connected) {
        if (_udpSocket->bind(QHostAddress::AnyIPv4, _multicastPort, QUdpSocket::ShareAddress)) {
            if (_udpSocket->joinMulticastGroup(_multicastAddress)) {
                _connected = true;
                updateStatusIndicator(_connected);

                ui->pushButton_Connect->setText("Отключиться");

                connect(_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readDatagram);
            }
        }
    } else {
        _udpSocket->leaveMulticastGroup(_multicastAddress);
        _udpSocket->close();

        disconnect(_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readDatagram);
        _connected = false;
        updateStatusIndicator(_connected);

        ui->pushButton_Connect->setText("Подключиться");
    }
}

void MainWindow::on_pushButton_ClearList_clicked()
{
    _storage.clear();

    updateTree();
    updateTotalCount();
}

void MainWindow::readDatagram() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        _udpSocket->readDatagram(datagram.data(), datagram.size());

        QJsonDocument doc = QJsonDocument::fromJson(datagram);

        if (!doc.isNull()) {
            qDebug().noquote() << "Получено: " << doc.toJson(QJsonDocument::Indented);
        }

        QJsonParseError error;
        doc = QJsonDocument::fromJson(datagram, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();

            QDate eventDate = QDate::fromString(obj["date"].toString(), "yyyy-MM-dd");
            QString eventText = obj["event"].toString();

            if (eventDate.isValid() && !eventText.isEmpty()) {
                _storage[eventDate].insert(eventText);

                updateTree();
                updateTotalCount();
            }
        }
    }
}

void MainWindow::updateTree() {
    ui->treeWidget->clear();

    for (auto it = _storage.begin(); it != _storage.end(); ++it) {
        QTreeWidgetItem *dateItem = new QTreeWidgetItem(ui->treeWidget);
        dateItem->setText(0, it.key().toString("yyyy-MM-dd"));

        QStringList events = it.value().values();
        events.sort();

        for (const QString &event : events) {
            QTreeWidgetItem *eventItem = new QTreeWidgetItem(dateItem);
            eventItem->setText(0, event);
        }
        dateItem->setExpanded(true);
    }
}

void MainWindow::updateTotalCount() {
    int total = 0;
    for (const QSet<QString>& events : _storage) {
        total += events.size();
    }
    ui->label_Count->setText("Всего событий: " + QString::number(total));
}

void MainWindow::updateStatusIndicator(bool success) {
    if (success) {
        ui->statusLabel->setStyleSheet(_baseStyle + "background-color: green;");
    } else {
        ui->statusLabel->setStyleSheet(_baseStyle + "background-color: red;");
    }
}
