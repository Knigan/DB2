#include "opendialog.h"
#include "ui_opendialog.h"

OpenDialog::OpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDialog) {
    ui->setupUi(this);
}

OpenDialog::~OpenDialog() {
    delete ui;
}

QString OpenDialog::get_host() {
    return ui->HostLineEdit->text();
}

int OpenDialog::get_port() {
    return ui->PortLineEdit->text().toInt();
}

QString OpenDialog::get_DatabaseName() {
    return ui->DBLineEdit->text();
}

QString OpenDialog::get_login() {
    return ui->LoginLineEdit->text();
}

QString OpenDialog::get_password() {
    return ui->PasswordLineEdit->text();
}

void OpenDialog::set_host(const QString& host) {
    ui->HostLineEdit->insert(host);
}

void OpenDialog::set_port(int port) {
    char buffer[128];
    itoa(port, buffer, 10);
    QString str(buffer);
    ui->PortLineEdit->insert(str);
}

void OpenDialog::set_DatabaseName(const QString& dbName) {
    ui->DBLineEdit->insert(dbName);
}

void OpenDialog::set_login(const QString& login) {
      ui->LoginLineEdit->insert(login);
}

void OpenDialog::set_password(const QString& password) {
     ui->PasswordLineEdit->insert(password);
}
