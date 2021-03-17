#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QSignalMapper>
#include <QSaveFile>
#include <QHostAddress>
#include <fstream>
#include <iostream>
#include "../src/common/Command.hpp"
#include "../src/Server/Network.hpp"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_CreateAccount_clicked();
    void on_Accout_Validation_clicked();
    void on_Connect_clicked();
    void on_Logout_clicked();
    void on_Help_clicked();

    void addContact(QString name, bool is_new);
    void setMainLabel(std::string text);

    void on_AddedNameContact_clicked();
    void on_Add_new_Contact_clicked();
    void on_Connect_with_IP_clicked();

    void on_Contactn_clicked(int n);

    void onReadyRead();
    void onReadyWrite(std::string cmd, std::vector<std::string> arg);
    void disconnected();

    void on_HangUp_clicked();

    void on_PickUp_clicked();

private:
    Ui::MainWindow *ui;

    std::string _uid;
    std::vector<std::string> _contactList;
    size_t _number_of_contact;
    std::string username;

    std::string address;
    int port;

    QTcpSocket _socket;

    bool is_calling;
    bool is_in_call;
    bool is_recieving_call;
};

#endif // MAINWINDOW_H
