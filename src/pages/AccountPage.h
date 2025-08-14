#pragma once

#include <QWidget>
#include "model/User.h"

class QLineEdit;
class QDoubleSpinBox;
class QPushButton;

class AccountPage : public QWidget {
	Q_OBJECT
public:
	explicit AccountPage(User &userRef, QWidget *parent = nullptr);

public slots:
	void reload();

private slots:
	void onSave();
	void onRecharge();

private:
	void setupUi();

private:
	User &m_user;
	QLineEdit *m_username{nullptr};
	QLineEdit *m_email{nullptr};
	QDoubleSpinBox *m_balance{nullptr};
	QPushButton *m_btnSave{nullptr};
	QPushButton *m_btnRecharge{nullptr};
};