#include "AccountPage.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>

AccountPage::AccountPage(User &userRef, QWidget *parent)
	: QWidget(parent), m_user(userRef) {
	setupUi();
	reload();
}

void AccountPage::setupUi() {
	auto *layout = new QVBoxLayout(this);
	auto *form = new QFormLayout();
	m_username = new QLineEdit(this);
	m_email = new QLineEdit(this);
	m_balance = new QDoubleSpinBox(this);
	m_balance->setRange(0.0, 1e9);
	m_balance->setDecimals(2);
	m_balance->setReadOnly(true);
	form->addRow(tr("用户名"), m_username);
	form->addRow(tr("邮箱"), m_email);
	form->addRow(tr("余额"), m_balance);
	layout->addLayout(form);

	auto *row = new QHBoxLayout();
	m_btnSave = new QPushButton(tr("保存资料"), this);
	m_btnRecharge = new QPushButton(tr("充值"), this);
	row->addWidget(m_btnSave);
	row->addWidget(m_btnRecharge);
	row->addStretch();
	layout->addLayout(row);

	connect(m_btnSave, &QPushButton::clicked, this, &AccountPage::onSave);
	connect(m_btnRecharge, &QPushButton::clicked, this, &AccountPage::onRecharge);
}

void AccountPage::reload() {
	m_username->setText(m_user.username);
	m_email->setText(m_user.email);
	m_balance->setValue(m_user.balance);
}

void AccountPage::onSave() {
	m_user.username = m_username->text();
	m_user.email = m_email->text();
	QMessageBox::information(this, tr("成功"), tr("已保存个人信息"));
}

void AccountPage::onRecharge() {
	m_user.balance += 100.0; // demo
	reload();
	QMessageBox::information(this, tr("成功"), tr("充值100.00成功"));
}