#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "eventstorage.h"
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_ClearList_clicked();
    void on_pushButton_Sync_clicked();
    void handleEvent(const QDate &date, const QString &event);

private:
    void setupUI();
    void updateTree();
    void updateStatus(bool connected);
    void updateTotalCount();

    Ui::MainWindow *ui;
    EventStorage *_storage;
    Network *_network;
    bool _connected;
};

#endif // MAINWINDOW_H
