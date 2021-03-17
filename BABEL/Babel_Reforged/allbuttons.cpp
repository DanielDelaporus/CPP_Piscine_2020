#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_CreateAccount_clicked() //Starting Account Creation
{
    QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("Warning");
    ui->stackedWidget->setCurrentIndex(1);
    _warning->setText(""); //Réinitialisation Warning
}

void MainWindow::on_Accout_Validation_clicked() //Validation de la création de compte
{
    QLineEdit* _login = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_login_2");
    QLineEdit* _psswd = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_pswd_2");
    QLineEdit* _psswd2 = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_pswd_3");
    QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("Warning_2");


    std::string log = _login->text().toUtf8().data();
    std::string pass = _psswd->text().toUtf8().data();

    if (_psswd->text() == _psswd2->text()) {
        std::cout << "RECIEVED" << std::endl;
        onReadyWrite("register", { log, pass });
        _warning->setText(""); //Réinitialisation Warning
        _psswd->setText("");
        _psswd2->setText("");
        _login->setText("");
    }
    else {
        _warning->setStyleSheet("background-color : rgba(0,0,0,0); color : red;");
        _warning->setText("Password isn't the same in both entry");
    }

}

void MainWindow::on_Connect_clicked() //Login
{
    QLineEdit* _login = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_login");
    QLineEdit* _psswd = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("_pswd");
    QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("Warning");

    std::string log = _login->text().toUtf8().data();
    std::string pass = _psswd->text().toUtf8().data();

    onReadyWrite("login", { log, pass });

    QLayout *ContactList = ui->stackedWidget->widget(2)->findChild<QLayout*>("verticalLayout");
    ContactList->setAlignment(Qt::AlignTop);

    _warning->setText(""); //Réinitialisation Warning
}

void MainWindow::on_Connect_with_IP_clicked()
{
    QLineEdit* IP = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("Ip_Entered");
    QLineEdit* Port = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("Ip_Entered_2");
    QLabel* _warning = ui->stackedWidget->currentWidget()->findChild<QLabel*>("warning_ip");


    try {
        address = IP->text().toUtf8().data();
        port = std::stoi(Port->text().toUtf8().data());
        _socket.connectToHost(QHostAddress(address.data()), port);
        connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(&_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    } catch (std::exception) {
        _warning->setStyleSheet("background-color : rgba(0,0,0,0); color : red;");
        _warning->setText("Connection has failed, invalid IP or port");
    }




    if (_socket.waitForConnected(1000)) {
        ui->stackedWidget->setCurrentIndex(0);
        _warning->setText(""); //Réinitialisation Warning
        IP->setText("");
        Port->setText("");
    }
    else {
        _warning->setStyleSheet("background-color : rgba(0,0,0,0); color : red;");
        _warning->setText("Connection has failed, invalid IP or port");
    }
}

void MainWindow::on_Logout_clicked()
{
    onReadyWrite("logout", {});
}

void MainWindow::on_Help_clicked()
{
    setMainLabel("Call using the contact list");
}

void MainWindow::on_Contactn_clicked(int n)
{
    if (is_calling != true && is_in_call != true && is_recieving_call != true) {
        QPushButton *new_ContactButton = ui->stackedWidget->widget(2)->findChild<QPushButton*>(std::to_string(n).data());
        std::string s = "Calling ";
        std::string ns = new_ContactButton->text().toUtf8().data();
        std::string end = "...";
        setMainLabel(s + ns+ end);
        onReadyWrite("call", { ns});
    }
}

void MainWindow::on_AddedNameContact_clicked()
{
    QLineEdit* nameContact = ui->stackedWidget->currentWidget()->findChild<QLineEdit*>("ContactName");

    onReadyWrite("addcontact", { nameContact->text().toUtf8().data() });
}

void MainWindow::on_Add_new_Contact_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
