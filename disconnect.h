#ifndef DISCONNECT_H
#define DISCONNECT_H

#include <QDialog>

namespace Ui {
class Disconnect;
}

class Disconnect : public QDialog {
    Q_OBJECT

public:
    explicit Disconnect(QWidget *parent = nullptr);
    ~Disconnect();

private:
    Ui::Disconnect *ui;
};

#endif // DISCONNECT_H
