#include "ProductDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDialogButtonBox>

ProductDialog::ProductDialog(QWidget *parent) : QDialog(parent) {
	setupUi();
}

void ProductDialog::setupUi() {
	auto *layout = new QVBoxLayout(this);
	auto *form = new QFormLayout();
	m_name = new QLineEdit(this);
	m_desc = new QLineEdit(this);
	m_price = new QDoubleSpinBox(this);
	m_price->setRange(0.0, 1000000.0);
	m_price->setDecimals(2);
	m_price->setSingleStep(0.5);
	m_stock = new QSpinBox(this);
	m_stock->setRange(0, 1000000);
	form->addRow(tr("名称"), m_name);
	form->addRow(tr("描述"), m_desc);
	form->addRow(tr("价格"), m_price);
	form->addRow(tr("库存"), m_stock);
	layout->addLayout(form);

	auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	layout->addWidget(buttons);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ProductDialog::setProduct(const Product &product) {
	m_current = product;
	m_name->setText(product.name);
	m_desc->setText(product.description);
	m_price->setValue(product.price);
	m_stock->setValue(product.stock);
}

Product ProductDialog::product() const {
	Product p = m_current;
	p.name = m_name->text();
	p.description = m_desc->text();
	p.price = m_price->value();
	p.stock = m_stock->value();
	return p;
}