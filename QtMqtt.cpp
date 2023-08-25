#include "QtMqtt.h"
#include "qmqtt.h"
#include "SetupDialog.h"
#include <QDebug>
#include <QHostInfo>


#define FONT_RED(s) "<font color=\"#FF0000\">" + QString(s) + "</font>"
#define FONT_GREEN(s) "<font color=\"#0AA000\">" + QString(s) + "</font>"
#define FONT_BLUE(s) "<font color=\"#0000FF\">" + QString(s) + "</font>"

QtMqtt::QtMqtt(QWidget *parent)
    : QMainWindow(parent),
    m_client(new QMQTT::Client)
{
    ui.setupUi(this);
    ui.messageList->setReadOnly(true);
    ui.messageList->append(FONT_GREEN("就绪"));

    serverSetupDialog = new SetupDialog(this);

    connect(m_client, &QMQTT::Client::connected, this, &QtMqtt::on_connected);
    connect(m_client, &QMQTT::Client::received, this, &QtMqtt::on_receivedMessage);
    connect(m_client, &QMQTT::Client::error, this, &QtMqtt::client_error_occured);
    connect(ui.subsList, &QListWidget::currentRowChanged, this, &QtMqtt::on_selectedSubChanged);
}

QtMqtt::~QtMqtt()
{}

void QtMqtt::on_startButton_clicked()
{
    if (serverSetupDialog->exec() == QDialog::Accepted)
    {
        ui.messageList->append("正在连接服务端......");

        //handle host
        auto serverip = QHostAddress(serverSetupDialog->getHost());
        if (!serverip.isNull())
        {
            m_client->setHost(serverip);
        }
        else
        {
            auto info = QHostInfo::fromName(serverSetupDialog->getHost());
            if (info.error() != QHostInfo::NoError)
			{
				ui.messageList->append(FONT_RED("无法解析域名，请检查域名或IP地址是否正确"));
				return;
			}
			m_client->setHost(info.addresses()[0]);
        }
        m_client->setPort(serverSetupDialog->getPort());
        m_client->setUsername(serverSetupDialog->getUsername());
        m_client->setPassword(serverSetupDialog->getPassword().toUtf8());
        
        m_client->connectToHost();
    }
}

void QtMqtt::on_stopButton_clicked()
{   
    m_client->disconnectFromHost();
    ui.messageList->append(FONT_RED("已断开连接"));
}

void QtMqtt::on_receivedMessage(const QMQTT::Message& msg)
{
    ui.messageList->append(QString::fromStdString(msg.payload().toStdString()));
}

void QtMqtt::on_connected()
{
	ui.messageList->append("连接成功");
}

void QtMqtt::on_clearButton_clicked()
{
    ui.messageList->clear();
}

void QtMqtt::on_newSubsButton_clicked()
{
    auto item_name = ui.subNameInput->text();
    if (item_name.isEmpty())return;

    auto items = ui.subsList->findItems(item_name, Qt::MatchExactly);
    if (items.empty())
    {
        ui.subsList->addItem(item_name);
        m_client->subscribe(item_name);
    }
    else
    {
        //no duplicate items, simply set the first one
        ui.subsList->setCurrentItem(items[0]);
	}
}

void QtMqtt::on_unsubscribeButton_clicked()
{
    auto subs = ui.subsList->findItems(ui.subNameInput->text(), Qt::MatchExactly);
    if (!subs.empty())
    {
        m_client->unsubscribe(subs[0]->text());
        //no duplicate items, simply delete the first one
        delete subs[0];
    }
}

void QtMqtt::on_selectedSubChanged(int row)
{
    if (row == -1)
    {
        ui.subNameInput->clear();

    }
    else
    {
        ui.subNameInput->setText(ui.subsList->item(row)->text());
    }
}

void QtMqtt::on_clearSubsButton_clicked()
{
    int subs_count = ui.subsList->count();
    for (int i = 0; i < subs_count; ++i)
    {
		m_client->unsubscribe(ui.subsList->item(i)->text());
	}
	ui.subsList->clear();
}

void QtMqtt::client_error_occured(QMQTT::ClientError error)
{
    switch (error)
    {
    case QMQTT::SocketConnectionRefusedError:
    case QMQTT::SocketRemoteHostClosedError:
    case QMQTT::SocketHostNotFoundError:
    case QMQTT::SocketAccessError:
    case QMQTT::SocketResourceError:
    case QMQTT::SocketTimeoutError:
    case QMQTT::SocketDatagramTooLargeError:
    case QMQTT::SocketNetworkError:
    case QMQTT::SocketAddressInUseError:
    case QMQTT::SocketAddressNotAvailableError:
    case QMQTT::SocketUnsupportedSocketOperationError:
    case QMQTT::SocketUnfinishedSocketOperationError:
    case QMQTT::SocketProxyAuthenticationRequiredError:
    case QMQTT::SocketSslHandshakeFailedError:
    case QMQTT::SocketProxyConnectionRefusedError:
    case QMQTT::SocketProxyConnectionClosedError:
    case QMQTT::SocketProxyConnectionTimeoutError:
    case QMQTT::SocketProxyNotFoundError:
    case QMQTT::SocketProxyProtocolError:
    case QMQTT::SocketOperationError:
    case QMQTT::SocketSslInternalError:
    case QMQTT::SocketSslInvalidUserDataError:
    case QMQTT::SocketTemporaryError:
    {
		ui.messageList->append(FONT_RED("网络错误，连接失败"));
		break;
    }
    case QMQTT::MqttUnacceptableProtocolVersionError:
    {
        ui.messageList->append(FONT_RED("MQTT协议版本不被接受，连接失败"));
        break;
    }
    case QMQTT::MqttBadUserNameOrPasswordError:
    case QMQTT::MqttNotAuthorizedError:
    {
        ui.messageList->append(FONT_RED("服务器需要身份验证，但未提供或提供了错误的用户名或密码，连接失败"));
        break;
    }
    default:
    {
		ui.messageList->append(FONT_RED("发生未知错误，连接失败"));
		break;
    }
    }
}