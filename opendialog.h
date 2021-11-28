#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>

namespace Ui {
class OpenDialog;
}

class OpenDialog : public QDialog {
    Q_OBJECT

public:
    explicit OpenDialog(QWidget *parent = nullptr);
    ~OpenDialog();
    QString get_host();
    int get_port();
    QString get_DatabaseName();
    QString get_login();
    QString get_password();
    void set_host(const QString&);
    void set_port(int);
    void set_DatabaseName(const QString&);
    void set_login(const QString&);
    void set_password(const QString&);

private:
    Ui::OpenDialog *ui;
};

#endif // OPENDIALOG_H
