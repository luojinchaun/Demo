#include "ProductPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>

#include "dialogs/ProductDialog.h"

ProductPage::ProductPage(QVector<Product> &productsRef, QWidget *parent)
	: QWidget(parent), m_products(productsRef) {
	setupUi();
	reload();
}

void ProductPage::setupUi() {
	auto *layout = new QVBoxLayout(this);
	m_table = new QTableWidget(this);
	m_table->setColumnCount(6);
	QStringList headers{tr("ID"), tr("名称"), tr("价格"), tr("库存"), tr("状态"), tr("描述")};
	m_table->setHorizontalHeaderLabels(headers);
	m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	layout->addWidget(m_table);

	auto *btnBar = new QHBoxLayout();
	m_btnAdd = new QPushButton(tr("上架(新增)"), this);
	m_btnEdit = new QPushButton(tr("修改"), this);
	m_btnDelete = new QPushButton(tr("删除"), this);
	m_btnToggleShelf = new QPushButton(tr("上下架"), this);
	m_btnAddToCart = new QPushButton(tr("加入购物车"), this);
	btnBar->addWidget(m_btnAdd);
	btnBar->addWidget(m_btnEdit);
	btnBar->addWidget(m_btnDelete);
	btnBar->addWidget(m_btnToggleShelf);
	btnBar->addWidget(m_btnAddToCart);
	btnBar->addStretch();
	layout->addLayout(btnBar);

	connect(m_btnAdd, &QPushButton::clicked, this, &ProductPage::onAdd);
	connect(m_btnEdit, &QPushButton::clicked, this, &ProductPage::onEdit);
	connect(m_btnDelete, &QPushButton::clicked, this, &ProductPage::onDelete);
	connect(m_btnToggleShelf, &QPushButton::clicked, this, &ProductPage::onToggleShelf);
	connect(m_btnAddToCart, &QPushButton::clicked, this, &ProductPage::onAddToCart);
}

void ProductPage::reload() {
	m_table->setRowCount(m_products.size());
	for (int row = 0; row < m_products.size(); ++row) {
		const Product &p = m_products[row];
		m_table->setItem(row, 0, new QTableWidgetItem(QString::number(p.id)));
		m_table->setItem(row, 1, new QTableWidgetItem(p.name));
		m_table->setItem(row, 2, new QTableWidgetItem(QString::number(p.price, 'f', 2)));
		m_table->setItem(row, 3, new QTableWidgetItem(QString::number(p.stock)));
		m_table->setItem(row, 4, new QTableWidgetItem(p.onShelf ? tr("在架") : tr("下架")));
		m_table->setItem(row, 5, new QTableWidgetItem(p.description));
	}
}

int ProductPage::selectedProductRow() const {
	auto items = m_table->selectedItems();
	if (items.isEmpty()) return -1;
	return items.first()->row();
}

Product *ProductPage::productByRow(int row) {
	if (row < 0 || row >= m_products.size()) return nullptr;
	return &m_products[row];
}

void ProductPage::onAdd() {
	ProductDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		Product p = dlg.product();
		// Assign new id
		int maxId = 0;
		for (const auto &it : m_products) maxId = std::max(maxId, it.id);
		p.id = maxId + 1;
		m_products.push_back(p);
		reload();
	}
}

void ProductPage::onEdit() {
	int row = selectedProductRow();
	Product *p = productByRow(row);
	if (!p) return;
	ProductDialog dlg(this);
	dlg.setProduct(*p);
	if (dlg.exec() == QDialog::Accepted) {
		*p = dlg.product();
		reload();
	}
}

void ProductPage::onDelete() {
	int row = selectedProductRow();
	if (row < 0) return;
	if (QMessageBox::question(this, tr("确认"), tr("删除该商品?")) == QMessageBox::Yes) {
		m_products.remove(row);
		reload();
	}
}

void ProductPage::onToggleShelf() {
	int row = selectedProductRow();
	Product *p = productByRow(row);
	if (!p) return;
	p->onShelf = !p->onShelf;
	reload();
}

void ProductPage::onAddToCart() {
	int row = selectedProductRow();
	Product *p = productByRow(row);
	if (!p || !p->onShelf || p->stock <= 0) return;
	bool ok = false;
	int qty = QInputDialog::getInt(this, tr("数量"), tr("请输入数量"), 1, 1, p->stock, 1, &ok);
	if (!ok) return;
	emit requestAddToCart(p->id, qty);
}