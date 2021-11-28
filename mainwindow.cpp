#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opendialog.h"
#include "disconnect.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);

    connect(ui->SendButton, SIGNAL(clicked()), this, SLOT(SendButton()));
    connect(ui->ClearButton, SIGNAL(clicked()), this, SLOT(ClearButton()));
    connect(ui->ClearHistoryButton, SIGNAL(clicked()), this, SLOT(ClearHistoryButton()));

    connection_settings = new QSettings("../connection_config.ini", QSettings::IniFormat, this);
    history_settings = new QSettings("../history_config.ini", QSettings::IniFormat, this);
    load_history();
    ConnectionInfo info;
    load_ConnectionInfo(info);
    DB = connect_to_Database(info);
    print_TablesList();
}

MainWindow::~MainWindow() {
    DB.close();
    delete ui;
    delete connection_settings;
    delete history_settings;
}

void MainWindow::on_action_triggered() {
    OpenDialog w;
    ConnectionInfo info;
    load_ConnectionInfo(info);
    w.set_host(info.host);
    w.set_port(info.port);
    w.set_DatabaseName(info.DatabaseName);
    w.set_login(info.login);
    w.set_password(info.password);
    if (w.exec() == QDialog::Accepted) {
        info.host = w.get_host();
        info.port = w.get_port();
        info.DatabaseName = w.get_DatabaseName();
        info.login = w.get_login();
        info.password = w.get_password();
    }
    DB = connect_to_Database(info);
    print_TablesList();
}

void MainWindow::on_actionDisconnect_triggered() {
    Disconnect dcon;
    if (dcon.exec() == QDialog::Accepted) {
        DB.removeDatabase("Database");
    }
    clear_TablesList();
}

QSqlDatabase MainWindow::connect_to_Database(ConnectionInfo &info) {
    QSqlDatabase DB = QSqlDatabase::addDatabase("QPSQL", "Database");
    DB.setHostName(info.host);
    DB.setPort(info.port);
    DB.setDatabaseName(info.DatabaseName);
    DB.setUserName(info.login);
    DB.setPassword(info.password);
    if (!DB.open()) {
       print_error("Database connection has failed!");
    } else {
        print_error("Database connection has been finished successfully!");
    }
    save_ConnectionInfo(info);
    return DB;
}

QString MainWindow::get_query() {
    return ui->CommandTextEdit->toPlainText();
}

void MainWindow::make_query(const QString& queryStr) {
    QSqlQuery* query = new QSqlQuery(DB);
    QSqlQueryModel *querymodel = new QSqlQueryModel;
    if (query->exec(queryStr)) {
        querymodel->setQuery(*query);
        ui->CurrentTable->setModel(querymodel);
        print_error("Query has processed successfully!");
        print_History(queryStr);
    } else if (queryStr.length() == 0) {
        print_error("Error: Query has been empty!");
    } else {
       print_error(query->lastError().text());
    }
}

void MainWindow::SendButton() {
    if (!DB.isValid()) {
        print_error("There is not connection to the database");
        return;
    }
    QString query = get_query();
    make_query(query);
}

void MainWindow::ClearButton() {
    ui->CommandTextEdit->clear();
    ui->ErrorLable->clear();
}

void MainWindow::ClearHistoryButton() {
    ui->History->clear();
    remove("../history_config.ini");
}

void MainWindow::print_TablesList() {
    QStringListModel* ptr = new QStringListModel(ui->TablesList);
    ptr->setStringList(DB.tables());
    ui->TablesList->setModel(ptr);
}

void MainWindow::print_History(const QString& query) {
    QString result;
    bool flag = true;
    for (int i = 0; i < query.length(); ++i) {
        if (query[i] == '\t' || query[i] == '\n' || query[i] == ' ') {
            if (flag) {
                result.push_back(' ');
                flag = false;
            }
        } else {
            result.push_back(query[i]);
            flag = true;
        }
    }
    ui->History->appendPlainText(result);
    save_history();
}

void MainWindow::print_error(const QString& error) {
   ui->ErrorLable->setText(error);
}

void MainWindow::save_ConnectionInfo(const ConnectionInfo &info) {
    connection_settings->setValue("Host", info.host);
    connection_settings->setValue("Port", info.port);
    connection_settings->setValue("DatabaseName", info.DatabaseName);
    connection_settings->setValue("Login", info.login);
    connection_settings->setValue("Password", info.password);
}

void MainWindow::load_ConnectionInfo(ConnectionInfo& info) {
    info.host = connection_settings->value("Host").toString();
    info.port = connection_settings->value("Port").toInt();
    info.DatabaseName = connection_settings->value("DatabaseName").toString();
    info.login = connection_settings->value("Login").toString();
    info.password = connection_settings->value("Password").toString();
}

void MainWindow::save_history() {
    QString str = ui->History->toPlainText();
    history_settings->setValue("history", str);
}

void MainWindow::load_history() {
    QString str = history_settings->value("history").toString();
    ui->History->appendPlainText(str);
}

void MainWindow::clear_TablesList() {
    QStringListModel* tables = new QStringListModel{};
    ui->TablesList->setModel(tables);
}

void MainWindow::on_TablesList_doubleClicked(const QModelIndex &index) {
    make_query("select * from " + index.data().toString() + ";");
}

