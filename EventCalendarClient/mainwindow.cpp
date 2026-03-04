#include "mainwindow.h"
#include "ui_mainwindow.h"

static const QString BASE_STYLE = "border-radius: 10px; border: 1px solid black;";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _storage = new EventStorage(this);
    _network = new Network(this);
    _connected = false;

    setupUI();

    connect(_network, &Network::eventReceived, this, &MainWindow::handleEvent);
}

MainWindow::~MainWindow() {
    if (_connected) {
        _network->stopReceiver();
    }

    delete ui;
}

void MainWindow::setupUI() {
    ui->statusLabel->setFixedSize(20, 20);
    ui->statusLabel->setStyleSheet(BASE_STYLE + "background-color: red;");
    ui->treeWidget->setHeaderLabel("Полученные события");
    ui->label_Address->setText("Адрес: 239.255.0.1");
    ui->label_Port->setText("Порт: 45454");
    updateTotalCount();
}

void MainWindow::on_pushButton_Connect_clicked() {
    if (!_connected) {
        if (_network->startReceiver()) {
            _connected = true;
            _network->setEnabled(true);
            updateStatus(true);
            ui->pushButton_Connect->setText("Отключиться");
        }
    } else {
        _network->stopReceiver();
        _network->setEnabled(false);
        _connected = false;
        updateStatus(false);
        ui->pushButton_Connect->setText("Подключиться");
    }
}

void MainWindow::on_pushButton_ClearList_clicked() {
    _storage->clear();
    updateTree();
    updateTotalCount();
}

void MainWindow::on_pushButton_Sync_clicked() {
    if (_connected) {
        _network->requestSync();
    }
}

void MainWindow::handleEvent(const QDate &date, const QString &event) {
    _storage->addEvent(date, event);
    updateTree();
    updateTotalCount();
}

void MainWindow::updateTree() {
    ui->treeWidget->clear();

    QMap<QDate, QSet<QString>> allEvents = _storage->getAllEvents();
    QList<QDate> dates = allEvents.keys();
    std::sort(dates.begin(), dates.end(), std::greater<QDate>());

    for (const QDate &date : dates) {
        QTreeWidgetItem *dateItem = new QTreeWidgetItem(ui->treeWidget);
        dateItem->setText(0, date.toString("yyyy-MM-dd"));

        QStringList events = allEvents[date].values();
        events.sort();

        for (const QString &event : events) {
            QTreeWidgetItem *eventItem = new QTreeWidgetItem(dateItem);
            eventItem->setText(0, event);
        }
        dateItem->setExpanded(true);
    }
}

void MainWindow::updateTotalCount() {
    ui->label_Count->setText("Всего событий: " + QString::number(_storage->totalCount()));
}

void MainWindow::updateStatus(bool connected) {
    QString color = connected ? "green" : "red";
    ui->statusLabel->setStyleSheet(BASE_STYLE + "background-color: " + color + ";");
}
