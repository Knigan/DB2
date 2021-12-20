#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opendialog.h"
#include "disconnect.h"
#include <ctime>

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch(event->key()) {
    case Qt::Key_Return:
        if (event->modifiers() == Qt::ControlModifier)
            SendRequest();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);

    QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, 0x01000005, Qt::ControlModifier);
    keyPressEvent(event);

    connection_settings = new QSettings("connection_config.ini", QSettings::IniFormat, this);
    history_settings = new QSettings("history_config.ini", QSettings::IniFormat, this);
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
        clear_TablesList();
    }
}

void MainWindow::on_actionClearEdit_triggered() {
    ui->CommandTextEdit->clear();
    ui->ErrorLable->clear();
}

void MainWindow::on_actionClearHistory_triggered() {
    ui->History->clear();
    remove("history_config.ini");
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
        if (!query->isSelect())
            print_TablesList();
    } else if (queryStr.length() == 0) {
        print_error("Error: Query has been empty!");
    } else {
       print_error(query->lastError().text());
    }
}

void MainWindow::SendRequest() {
    if (!DB.isValid()) {
        print_error("There is not connection to the database");
        return;
    }
    QString query = get_query();
    make_query(query);
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
    make_query("SELECT * FROM " + index.data().toString() + ";");
}

void MainWindow::on_actionCreateMenTable_triggered() {
    make_query("CREATE TABLE kn_MenTable (id int NOT NULL, name text PRIMARY KEY);");
    QString str = "INSERT INTO kn_MenTable(id, name) VALUES ";
    std::vector<QString> vec = {"Аббас", "Абд", "Абдуллах", "Абид", "Абу", "Адам", "Адиль", "Азиз", "Азим", "Акиф", "Акиль", "Акрам", "Али", "Алим", "Амин", "Амир", "Анас", "Анвар", "Ариф",
                                "Асад", "Асаф", "Ахмад", "Бадр", "Басиль", "Басир", "Билял", "Бурхан", "Вагиз", "Вазир", "Валид", "Вахид", "Гази", "Гайс", "Галиб", "Гариб", "Гафур",
                                "Дани", "Дари", "Джабир", "Джавад", "Джамиль", "Джарир", "Джафар", "Джихад", "Джума", "Динар", "Захид", "Захир", "Заид", "Закария", "Заки", "Зайн", "Зариф",
                                "Зафар", "Зейд", "Зирьяб", "Ибрахим", "Идрис", "Иззуддин", "Имран", "Иршад", "Иса", "Искандер", "Ислам", "Исмаил", "Кадир", "Казим", "Камиль", "Карим",
                                "Кахтан", "Курбан", "Латиф", "Маджид", "Мади", "Маймун", "Малик", "Мансур", "Масуд", "Махди", "Махмуд", "Машхур", "Минхадж", "Муаз", "Мумин", "Мубарак",
                                "Муджахид", "Музаффар", "Мукбиль", "Мукрин", "Мунтасир", "Мурад", "Муса", "Муслим", "Мустафа", "Мухаджир", "Мухаммад", "Мухаррам", "Мухтар",
                                "Надим", "Надр", "Наиль", "Насиф", "Низар", "Рамадан", "Рами", "Рамиль", "Рашид", "Ридван", "Ризк", "Саад", "Сабит", "Сабих", "Саддам", "Садик", "Саид",
                                "Сайф", "Салах", "Салих", "Салман", "Самир", "Сулейман", "Талиб", "Убар", "Усама", "Усман", "Фазиль", "Фаик", "Фарадж", "Фарид", "Фахим", "Фуад", "Фарах",
                                "Хабиб", "Хади", "Хадир", "Хайдар", "Хайри", "Хайям", "Хаким", "Халид", "Халил", "Халим", "Хам", "Хамид", "Хамза", "Хаммам", "Хани", "Харис", "Хасан",
                                "Хатим", "Хаттаб", "Хафиз", "Хафс", "Хашим", "Хидр", "Хиляль", "Хишам", "Хусейн", "Шаабан", "Шади", "Шакир", "Шамиль", "Шахид", "Шуайб", "Якуб", "Яруб"};
    for (size_t i = 0; i < vec.size(); ++i) {
        QString number;
        str += "(" + number.setNum(i + 1) + ", '" + vec[i] + "')";
        if (i + 1 == vec.size())
            str += ";";
        else
            str += ", ";
    }
    make_query(str);
}

void MainWindow::on_actionCreateWomenTable_triggered() {
    make_query("CREATE TABLE kn_WomenTable (id int NOT NULL, name text PRIMARY KEY);");
    QString str = "INSERT INTO kn_WomenTable(id, name) VALUES ";
    std::vector<QString> vec = {"Агафья", "Аглая", "Агния", "Агриппина", "Аза", "Акулина", "Алевтина", "Александра", "Алина", "Алла", "Анастасия", "Ангелина", "Анжела", "Анжелика",
                                "Анна", "Антонина", "Анфиса", "Валентина", "Валерия", "Варвара", "Василиса", "Вера", "Вероника", "Виктория", "Галина", "Глафира", "Гликерия", "Дана",
                                "Дарья", "Евгения", "Евдокия", "Евлалия", "Евлампия", "Евпраксия", "Евфросиния", "Екатерина", "Елена", "Елизавета", "Епистима", "Ермиония", "Жанна",
                                "Зинаида", "Злата", "Зоя", "Инга", "Инесса", "Инна", "Иоанна", "Ираида", "Ирина", "Ия", "Капитолина", "Карина", "Каролина", "Кира", "Клавдия", "Ксения",
                                "Лада", "Лариса", "Лидия", "Лилия", "Любовь", "Людмила", "Маргарита", "Марина", "Мария", "Марфа", "Матрёна", "Милица", "Мирослава", "Надежда", "Наталья",
                                "Нина", "Нонна", "Оксана", "Октябрина", "Олимпиада", "Ольга", "Павлина", "Пелагея", "Пинна", "Полина", "Прасковья", "Рада", "Раиса", "Римма", "Светлана",
                                "Серафима", "Снежана", "София", "Таисия", "Тамара", "Татьяна", "Улита", "Ульяна", "Урсула", "Фаина", "Феврония", "Фёкла", "Феодора", "Целестина",
                                "Юлия", "Яна", "Ярослава"};
    for (size_t i = 0; i < vec.size(); ++i) {
        QString number;
        str += "(" + number.setNum(i + 1) + ", '" + vec[i] + "')";
        if (i + 1 == vec.size())
            str += ";";
        else
            str += ", ";
    }
    make_query(str);
}

void MainWindow::on_actionDropMenTable_triggered() {
    make_query("DROP TABLE kn_MenTable;");
}

void MainWindow::on_actionDropWomenTable_triggered() {
    make_query("DROP TABLE kn_WomenTable;");
}

void MainWindow::on_actionCreateMeetingsTable_triggered() {
    make_query("CREATE TABLE kn_MeetingsTable "
                  "(id int NOT NULL PRIMARY KEY, time text, man text, woman text, "
                  "FOREIGN KEY (man) references kn_MenTable(name) on DELETE CASCADE on UPDATE CASCADE, "
                  "FOREIGN KEY (woman) references kn_WomenTable(name) on DELETE CASCADE on UPDATE CASCADE);");
    make_query("SELECT meetings();");
}

void MainWindow::on_actionDropMeetingsTable_triggered() {
    make_query("DROP TABLE kn_MeetingsTable;");
}

void MainWindow::on_actionSearch_triggered() {
    QString str = ui->CommandTextEdit->toPlainText();
    if (str.length() != 0) {
        make_query("DROP INDEX IF EXISTS idx;");
        make_query("SELECT * FROM kn_meetingstable WHERE man = '" + str + "';");
    }
    else make_query("SELECT * FROM kn_meetingstable;");
}

void MainWindow::on_actionIndexSearch_triggered() {
    QString str = ui->CommandTextEdit->toPlainText();
    if (str.length() != 0) {
        make_query("CREATE INDEX idx ON kn_meetingstable USING GIN(to_tsvector('russian', man));");
        make_query("SELECT * FROM kn_MeetingsTable WHERE to_tsvector('russian', man) @@ to_tsquery('russian', '" + str +  "');");
    }
}

void MainWindow::on_actionAnalyzeSearch_triggered() {
    QString str = ui->CommandTextEdit->toPlainText();
    if (str.length() != 0) {
        make_query("DROP INDEX IF EXISTS idx;");
        make_query("EXPLAIN ANALYZE SELECT * FROM kn_meetingstable WHERE man = '" + str + "';");
    }
    else make_query("EXPLAIN ANALYZE SELECT * FROM kn_meetingstable;");
}

void MainWindow::on_actionAnalyzeIndexSearch_triggered() {
    QString str = ui->CommandTextEdit->toPlainText();
    if (str.length() != 0) {
        make_query("CREATE INDEX idx ON kn_meetingstable USING GIN(to_tsvector('russian', man));");
        make_query("EXPLAIN ANALYZE SELECT * FROM kn_MeetingsTable WHERE to_tsvector('russian', man) @@ to_tsquery('russian', '" + str +  "');");
    }
}

