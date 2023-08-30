#include "SetupDialog.h"
#include <QMessagebox>

SetupDialog::SetupDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.portInput->setValidator(new QIntValidator(1, 65535, this));
	connect(this->ui.connectButton, &QPushButton::clicked, this, &SetupDialog::applySettings);
}


SetupDialog::~SetupDialog()
{}

QString SetupDialog::getHost()
{
	return host;
}

quint16 SetupDialog::getPort()
{
	return port;
}
QString SetupDialog::getUsername()
{
	return username;
}

QString SetupDialog::getPassword()
{
	return password;
}

quint8 SetupDialog::getNetworkProtocol()
{
	return networkProtocol;
}

bool SetupDialog::is_tls_enabled()
{
	return tls_enable;
}

void SetupDialog::applySettings()
{
	host = ui.hostInput->text();
	if (host.isEmpty())
	{
		QMessageBox::critical(this, "Error", "地址不能为空");
		return;
	}
	if (ui.portInput->hasAcceptableInput())
	{
		port = ui.portInput->text().toUShort();
	}
	else
	{
		QMessageBox::critical(this, "Error", "端口号不合法，请输入1-65535之间的整数");
		return;
	}
	username = ui.usernameInput->text();
	password = ui.passwordInput->text();
	networkProtocol = ui.protocolSelector->currentIndex();
	tls_enable = ui.tlsCheckBox->isChecked();
	this->accept();
}


