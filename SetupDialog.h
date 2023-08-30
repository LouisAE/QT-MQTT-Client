#pragma once

#include <QDialog>
#include <QValidator>
#include "ui_SetupDialog.h"

#define NETWORK_PROTOCOL_TCP 0

class SetupDialog  : public QDialog
{
	Q_OBJECT

public:
	SetupDialog(QWidget *parent=nullptr);
	~SetupDialog();

	QString getHost();
	quint16 getPort();
	QString getUsername();
	QString getPassword();
	quint8 getNetworkProtocol();
	bool is_tls_enabled();

private slots:
	void applySettings();
private:
	Ui::SetupDialog ui;

	QString host;
	quint16 port;
	QString username;
	QString password;
	quint8 networkProtocol;
	bool tls_enable;

	
};
