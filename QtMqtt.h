#pragma once

#include <QtWidgets/QMainWindow>
#include "qmqtt.h"
#include <QListWidgetItem>
#include "ui_QtMqtt.h"

class SetupDialog;

class QtMqtt : public QMainWindow
{
    Q_OBJECT

public:
    QtMqtt(QWidget *parent = nullptr);
    ~QtMqtt();

private:
    Ui::QtMqttClass ui;
    QMQTT::Client *m_client;

    SetupDialog *serverSetupDialog;

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_receivedMessage(const QMQTT::Message& msg);
    void on_connected();
    void on_clearButton_clicked();
    void on_newSubsButton_clicked();
    void on_unsubscribeButton_clicked();
    void on_selectedSubChanged(int row);
    void on_clearSubsButton_clicked();
    void client_error_occured(QMQTT::ClientError error);
};
