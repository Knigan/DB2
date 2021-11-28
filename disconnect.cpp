#include "disconnect.h"
#include "ui_disconnect.h"

Disconnect::Disconnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Disconnect) {
    ui->setupUi(this);
}

Disconnect::~Disconnect() {
    delete ui;
}
