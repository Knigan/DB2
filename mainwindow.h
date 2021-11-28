#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QDebug>
#include <QStringListModel>
#include <QSettings>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct ConnectionInfo {
    QString host;
    int port;
    QString DatabaseName;
    QString login;
    QString password;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();
    void on_actionDisconnect_triggered();
    void SendButton();
    void ClearButton();
    void ClearHistoryButton();
    void on_TablesList_doubleClicked(const QModelIndex&);

private:
    Ui::MainWindow *ui;
    QSqlDatabase DB;
    QSettings* connection_settings;
    QSettings* history_settings;
    QSqlDatabase connect_to_Database(ConnectionInfo&);
    void disconnect();
    QString get_query();
    void make_query(const QString&);
    void print_TablesList();
    void print_History(const QString&);
    void print_error(const QString&);
    void save_ConnectionInfo(const ConnectionInfo&);
    void load_ConnectionInfo(ConnectionInfo&);
    void save_history();
    void load_history();
    void clear_TablesList();
};

#endif // MAINWINDOW_H
