#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMap>
#include <QSet>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateTree();
    void updateTotalCount();
    void updateStatusIndicator(bool success);

private slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_ClearList_clicked();   
    void on_pushButton_Sync_clicked();
    void readDatagram();
    void sendRequest();

private:
    Ui::MainWindow *ui;
    QMap<QDate, QSet<QString>> _storage;
    QUdpSocket *_udpSocket;
    QUdpSocket *_syncSocket;
    QHostAddress _multicastAddress;
    quint16 _multicastPort1;
    quint16 _multicastPort2;
    bool _connected;
};
#endif // MAINWINDOW_H
