#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _socket(this)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(4);
    _number_of_contact = 0;
    _uid = "";
    is_in_call = false;
    is_recieving_call = false;
    is_calling = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addContact(QString name, bool is_new)
{
    QLayout *ContactList = ui->stackedWidget->widget(2)->findChild<QLayout*>("verticalLayout");
    QPushButton *new_ContactButton = ui->stackedWidget->widget(2)->findChild<QPushButton*>("Add_new_Contact");
    QPushButton *newContact = new QPushButton();

    ContactList->setAlignment(Qt::AlignTop);
    newContact->setStyleSheet(new_ContactButton->styleSheet());

    if (is_new)
        _number_of_contact++;

    newContact->setObjectName(std::to_string(_number_of_contact).data());
    int n = _number_of_contact;
    connect(newContact, &QPushButton::clicked, [this, n] { on_Contactn_clicked(n); });

    newContact->setText(name);
    ContactList->addWidget(newContact);
}

void MainWindow::setMainLabel(std::string text)
{
    QLabel* _warning = ui->stackedWidget->widget(2)->findChild<QLabel*>("CallLabel");
    _warning->setStyleSheet("font-size:36pt; color:#2e3436;");
    _warning->setAlignment(Qt::AlignCenter);
    QString str = QString::fromUtf8(text.c_str());
    _warning->setText(str);
}

void MainWindow::onReadyRead()
{
    QByteArray datas = _socket.readAll();
    QSaveFile file("test.txt");
    file.open(QIODevice::WriteOnly);
    file.write(datas);
    file.commit();

    Command _cmd;
    {
        try {
            std::ifstream _is("test.txt");
            boost::archive::binary_iarchive ia(_is);
            ia >> _cmd;
        } catch (std::exception) {
            _cmd = Command();
        }
    }




    if (_cmd.getcmd() == "r_register") {
        QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("Warning_2");
        if (_cmd.getarg()[0] == "already used") {
            _warning->setStyleSheet("background-color : rgba(0,0,0,0); color : red;");
            _warning->setText("Login already in use");
        }
        else {
            ui->stackedWidget->setCurrentIndex(0);
        }
    }




    if (_cmd.getcmd() == "r_login") {
        QLineEdit* _login = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_login");
        QLineEdit* _psswd = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_pswd");
        QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("Warning");

        if (_cmd.getarg()[0] == "error") {
            _warning->setStyleSheet("background-color : rgba(0,0,0,0); color : red;");
            _warning->setText("Login or password is wrong");
        }
        else {
            username = _login->text().toUtf8().data();
            _uid = _cmd.getarg()[0];
            is_in_call = false;
            is_recieving_call = false;
            is_calling = false;
            _psswd->setText("");
            _login->setText("");
            onReadyWrite("getcontact", {});
            ui->stackedWidget->setCurrentIndex(2);
        }
    }




    if (_cmd.getcmd() == "r_getcontact") {
        _contactList = _cmd.getarg();
        _number_of_contact = _cmd.getarg().size();
        for (size_t i = 0; i < _number_of_contact ; i++) {
            QString tmp = QString::fromUtf8(_contactList[i].c_str());
            addContact(tmp, 0);
        }
    }




    if (_cmd.getcmd() == "r_logout") {
        QLayout *ContactList = ui->stackedWidget->widget(2)->findChild<QLayout*>("verticalLayout");
        ui->stackedWidget->setCurrentIndex(0);
        _uid = "";
        while (ContactList->count()>0)
        {
          delete ContactList->takeAt(0);
        }
        username = "";
        _number_of_contact = 0;
        _contactList = {};
    }


    if (_cmd.getcmd() == "r_addcontact") {
        if (_cmd.getarg()[0] == "ok") {
            QLineEdit* nameContact = ui->stackedWidget->widget(3)->findChild<QLineEdit*>("ContactName");
            addContact(nameContact->text(), true);
            nameContact->setText("");
            ui->stackedWidget->setCurrentIndex(2);
        }
    }

    if (_cmd.getcmd() == "ask_answer") {

        if (is_calling || is_in_call) {
            //Nope
        }
        else {
            QPushButton *pick_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("PickUp");
            QPushButton *hang_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("HangUp");
            //DRING DRING
            hang_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(164, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px;\
                border-radius: 5px;\
                border-color:  rgb(164, 0, 0);\
                border-style: outset;\
                color: rgb(255, 255, 255);");

           pick_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(78, 154, 6, 255), stop:1 rgba(255, 255, 255, 255));\
                   border-width: 2px; \
                   border-radius: 5px; \
                   border-color: rgb(78, 154, 6); \
                   border-style: outset; \
                   color:  rgb(255, 255, 255); ");

            std::string tmp = "Recieving a call from ";
            setMainLabel(tmp + _cmd.getarg()[1]);
            is_recieving_call = true;
        }
    }


    if (_cmd.getcmd() == "r_hangup ") {
        QPushButton *hang_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("HangUp");
        is_in_call = false;
        hang_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px;\
                border-radius: 5px;\
                border-color:  rgb(164, 0, 0);\
                border-style: outset;\
                color: rgb(164, 0, 0);");
        setMainLabel("End of Call");
    }

    if (_cmd.getcmd() == "r_send_ip ") {
        is_calling = false;
        is_in_call = true;
        //GET IP AND PORT
        std::string other_ip = _cmd.getarg()[0];
        std::string other_port = _cmd.getarg()[1];
        setMainLabel("On call...");
    }

    if (_cmd.getcmd() == "r_call_reject ") {
        QPushButton *hang_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("HangUp");
        is_calling = false;
        hang_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px;\
                border-radius: 5px;\
                border-color:  rgb(164, 0, 0);\
                border-style: outset;\
                color: rgb(164, 0, 0);");

        setMainLabel("Call turned down");
    }
}

void MainWindow::onReadyWrite(std::string cmd, std::vector<std::string> arg)
{
    QByteArray datas;

    std::ofstream buf("test_1.txt");

    Command bis(cmd, arg, _uid);
    {
        boost::archive::binary_oarchive oa(buf);
        oa << bis;
        oa << "\0\0\0";
    }
    QFile file("test_1.txt");
    file.open(QIODevice::ReadOnly);
    datas = file.read(file.size());
    _socket.write(datas);
}

void MainWindow::disconnected()
{
    QLayout *ContactList = ui->stackedWidget->widget(2)->findChild<QLayout*>("verticalLayout");
    _uid = "";
    while (ContactList->count()>0)
    {
      delete ContactList->takeAt(0);
    }
    username = "";
    _number_of_contact = 0;
    _contactList = {};
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_HangUp_clicked()
{
    QPushButton *pick_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("PickUp");
    QPushButton *hang_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("HangUp");
    if (is_recieving_call)
    {
        is_recieving_call = false;
        pick_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px; \
                border-radius: 5px; \
                border-color: rgb(78, 154, 6); \
                border-style: outset; \
                color:  rgb(78, 154, 6); ");
        hang_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px;\
                border-radius: 5px;\
                border-color:  rgb(164, 0, 0);\
                border-style: outset;\
                color: rgb(164, 0, 0);");

        setMainLabel("Call turned down");
        onReadyWrite("answer", {"ko"});
    }
    else if (is_in_call) {
        is_in_call = false;
        hang_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px;\
                border-radius: 5px;\
                border-color:  rgb(164, 0, 0);\
                border-style: outset;\
                color: rgb(164, 0, 0);");
        setMainLabel("End of Call");
        onReadyWrite("hangup", {});
    }

}

void MainWindow::on_PickUp_clicked()
{
    QPushButton *pick_up = ui->stackedWidget->widget(2)->findChild<QPushButton*>("PickUp");
    if (is_recieving_call == true)
    {
        is_in_call = true;
        is_recieving_call = false;
        pick_up->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(255, 255, 255, 255));\
                border-width: 2px; \
                border-radius: 5px; \
                border-color: rgb(78, 154, 6); \
                border-style: outset; \
                color:  rgb(78, 154, 6); ");

        setMainLabel("On call...");
        onReadyWrite("answer", {"ok", "", ""});
    }
}
